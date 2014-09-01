#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <core/condition_var.h>
#include <core/mutex.h>
#include <core/stretchy_buffer.h>
#include <core/thread.h>
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <kazmath/kazmath.h>
#include <log/log.h>

#include "camera.h"
#include "gl_utils.h"
#include "khash.h"
#include "sprite.h"
#include "texture.h"

typedef struct renderer {
        GLFWwindow* window;

        uint16_t width;
        uint16_t height;
        uint16_t virtual_width;
        uint16_t virtual_height;

        GLuint shader_program;
        GLuint vert_attrib;
        GLuint tex_coord_attrib;
        uint32_t tex_unit;

        // Rendering is double buffered. So while gameplay thread writes new
        // data the rendering thread can render from the other buffer.
        sprite* sprite_sb[2];
        uint8_t current_buffer; // The buffer new sprites can be added to.

        thread* render_thread;
        mutex* render_mutex;
        condition_var* render_condition;
        bool rendering;
        bool done;
} renderer;

#define CHECG_GL

int compare_sprites(const void* lhs, const void* rhs);
uint32_t __stdcall render_func(void* renderer);
void swap_sprite_sb(renderer* r);
sprite* prepare_back_buffer(renderer*);
void render_sprites(renderer* r, sprite* sprites_sb);
float* calc_verts(sprite* s, float* vert_buffer);
float* calc_tex_coords(sprite* s, float* tex_coord_buffer);
void draw_buffers(renderer*, float* vert_buffer, float* tex_coord_buffer);
bool upload_texture(renderer* r, texture* t);

void bindTextureUnit(uint32_t shader_prog,
                     uint32_t tex_unit,
                     const char* uniform_name)
{
        GLuint tex_uniform = glGetUniformLocation(shader_prog, uniform_name);
        glUseProgram(shader_prog);
        glUniform1i(tex_uniform, tex_unit);
        glUseProgram(0);
}

void switchTexture(renderer* r, texture* t)
{
        assert(t);

        glActiveTexture(GL_TEXTURE0 + r->tex_unit);
        glBindTexture(GL_TEXTURE_2D, t->gl_id);
}

void bindSampler(uint32_t tex_unit)
{
        uint32_t sampler;
        glGenSamplers(1, &sampler);
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glBindSampler(tex_unit, sampler);
}

bool check_gl_error()
{
        bool error = false;
        GLenum gl_error = glGetError();
        while (gl_error != GL_NO_ERROR) {
                LOGERR("GL error occurred: %d", gl_error);
                error = true;
                gl_error = glGetError();
        }

        return error;
}

renderer* render_create(GLFWwindow* window,
                        uint32_t virtual_width, uint32_t virtual_height,
                        const char* vert_shader_path, const char* frag_shader_path)
{
        assert(window);

        renderer* r = malloc(sizeof(*r));
        if (!r) {
                LOGERR("%s", "Failed to allocate renderer");
                goto return_failed;
        }

        GLuint vert_shader = make_shader(GL_VERTEX_SHADER, vert_shader_path);
        if (vert_shader == 0) {
                LOGERR("%s", "Error making vertex shader");
                goto cleanup_renderer;
        }

        GLuint frag_shader = make_shader(GL_FRAGMENT_SHADER, frag_shader_path);
        if (frag_shader == 0) {
                LOGERR("%s", "Error making fragment shader");
                goto cleanup_vert;
        }

        r->shader_program = make_program(vert_shader, frag_shader);
        if (r->shader_program == 0) {
                LOGERR("%s", "Error making shader program");
                goto cleanup_frag;
        }

        r->render_mutex = mutex_create();
        if (!r->render_mutex) {
                LOGERR("%s", "Failed to allocate render mutex");
                goto cleanup_shader_prog;
        }

        r->render_condition = condition_var_create();
        if (!r->render_condition) {
                LOGERR("%s", "Failed to allocate condition variable");
                goto cleanup_render_mutex;
        }

        r->window = window;

        r->sprite_sb[0] = NULL;
        r->sprite_sb[1] = NULL;
        r->rendering = false;
        r->done = false;
        r->vert_attrib = glGetAttribLocation(r->shader_program, "vertex");
        r->tex_coord_attrib = glGetAttribLocation(r->shader_program, "tex_coord");
        r->virtual_width = virtual_width;
        r->virtual_height = virtual_height;
        r->tex_unit = 0;
        r->current_buffer = 0;

        bindTextureUnit(r->shader_program, r->tex_unit, "sprite_texture");
        bindSampler(r->tex_unit);
        uint32_t width, height;
        glfwGetWindowSize(window, &width, &height);
        render_resize(r, width, height);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        r->render_thread = thread_create("render_thread", render_func, r);
        if (!r->render_thread) {
                LOGERR("%s", "Failed to create render_thread");
                goto cleanup_condition_var;
        }

        if (check_gl_error()) {
                goto cleanup_render_thread;
        }

        return r;

cleanup_render_thread:
        mutex_lock(r->render_mutex);
        r->done = true;
        condition_var_notify(r->render_condition);
        mutex_unlock(r->render_mutex);
        thread_join(r->render_thread);
        thread_free(r->render_thread);
cleanup_condition_var:
        condition_var_free(r->render_condition);
cleanup_render_mutex:
        mutex_free(r->render_mutex);
cleanup_shader_prog:
        glDeleteProgram(r->shader_program);
cleanup_frag:
        glDeleteShader(frag_shader);
cleanup_vert:
        glDeleteShader(vert_shader);
cleanup_renderer:
        free(r);
return_failed:
        return NULL;
}

void render_free(renderer* r)
{
        assert(r);

        // Stop the thread
        mutex_lock(r->render_mutex);
        r->done = true;
        condition_var_notify(r->render_condition);
        mutex_unlock(r->render_mutex);
        thread_join(r->render_thread);
        thread_free(r->render_thread);

        condition_var_free(r->render_condition);
        mutex_free(r->render_mutex);

        glfwMakeContextCurrent(r->window);
        glDeleteProgram(r->shader_program);
        free(r);
}

void render_resize(renderer* r, uint32_t screen_width, uint32_t screen_height)
{
        assert(r);

        r->width = screen_width;
        r->height = screen_height;
        float aspect_ratio = ((float)r->virtual_width) / ((float)r->virtual_height);
        uint32_t width = screen_width;
        uint32_t height = (uint32_t)(width / aspect_ratio + 0.5f);

        if (height > screen_height) {
                // We have to use pillar box rather than letterbox.
                height = screen_height;
                width = (uint32_t)(height * aspect_ratio + 0.5f);
        }

        uint32_t viewport_x = (screen_width / 2) - (width / 2);
        uint32_t viewport_y = (screen_height / 2) - (height / 2);

        glViewport(viewport_x, viewport_y, width, height);

        GLint proj_uniform = glGetUniformLocation(r->shader_program, "projection");
        kmMat4 proj_matrix;
        kmMat4OrthographicProjection(&proj_matrix,
                                     0, (float)r->virtual_width,
                                     0, (float)r->virtual_height,
                                     -1, 1);
        glUseProgram(r->shader_program);
        glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, proj_matrix.mat);
        glUseProgram(0);

        if (check_gl_error()) {
                LOGERR("%s", "An GL error occurred when resizing renderer");
        }
}

void render_add_sprite(renderer* r, const sprite* s)
{
        assert(s);
        sb_push(r->sprite_sb[r->current_buffer], *s);
}

void render_add_sprites(renderer* r, const sprite* sprites, 
                        int32_t sprites_len)
{
        assert(r);
        sprite* begin = sb_add(r->sprite_sb[r->current_buffer], sprites_len);
        memcpy(begin, sprites, sprites_len * sizeof(sprite));
}

void render_delete_texture(renderer* r, texture* t)
{
        assert(r);
        assert(t);

        glfwMakeContextCurrent(r->window);
        glDeleteTextures(1, &t->gl_id);
}

void render_submit(renderer* r)
{
        assert(r);

        mutex_lock(r->render_mutex);
        while (r->rendering) {
                condition_var_wait(r->render_condition, r->render_mutex);
        }
        mutex_unlock(r->render_mutex);

        glfwMakeContextCurrent(NULL);
        swap_sprite_sb(r);

        // Tell the rendering thread to go.
        condition_var_notify(r->render_condition);
}

int compare_sprites(const void* lhs, const void* rhs)
{
        const sprite* a = lhs;
        const sprite* b = rhs;

        if (a->depth == b->depth) {
                return b->tex->id - a->tex->id;
        }
        
        return b->depth - a->depth;
}

uint32_t __stdcall render_func(void* data)
{
        renderer* r = (renderer*)data;

        while (!r->done) {
                mutex_lock(r->render_mutex);
                // Tell the game play thread rendering done.
                r->rendering = false;
                condition_var_notify(r->render_condition);

                // Wait for the game play thread to kick off rendering.
                condition_var_wait(r->render_condition, r->render_mutex);
                if (r->done) {
                        return 0;
                }
                r->rendering = true;
                glfwMakeContextCurrent(r->window);
                mutex_unlock(r->render_mutex);

                sprite* sprites = prepare_back_buffer(r);
                render_sprites(r, sprites);
                glfwSwapBuffers(r->window);
                sb_reset(sprites);

                if (check_gl_error()) {
                        LOGERR("%s", "An GL error occurred when rendering");
                }
        }

        return 0;
}

void swap_sprite_sb(renderer* r)
{
        r->current_buffer = ++r->current_buffer % 2;
}

// Gets the back buffer and sorts it. Returns a 
// pointer to the sorted buffer.
sprite* prepare_back_buffer(renderer* r)
{
        // Find the buffer to read from.
        uint8_t back_buffer = (r->current_buffer + 1) % 2;
        sprite* sprites = r->sprite_sb[back_buffer];

        // Sort the sprites by texture ID to minimize the number of
        // texture switches we have to do.
        qsort(sprites, sb_count(sprites), sizeof(sprite), compare_sprites);

        return sprites;
}

void render_sprites(renderer* r, sprite* sprites_sb)
{
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int num_sprites = sb_count(sprites_sb);
        if (num_sprites == 0) {
                return;
        }

        glUseProgram(r->shader_program);

        // Set the camera transfrom
        GLint cam_uniform = glGetUniformLocation(r->shader_program, "cam");
        kmMat4* cam_mat = cam_transform();
        glUniformMatrix4fv(cam_uniform, 1, GL_FALSE, cam_mat->mat);

        // Sprites are sorted by texture ID so render until texture 
        // switch then render some more. Kick off the first sprite.
        float* vert_sb = NULL;
        float* tex_coord_sb = NULL; // todo hoist up to renderer to avoid allocs
        for (int i = 0; i < num_sprites; ++i) {
                sprite* s = &sprites_sb[i];
                sprite* next = NULL;
                if (i < num_sprites - 1) {
                        next = &sprites_sb[i + 1];
                }

                vert_sb = calc_verts(s, vert_sb);
                tex_coord_sb = calc_tex_coords(s, tex_coord_sb);

                if (next == NULL || s->tex->id != next->tex->id) {
                        // switch to new texture and draw
                        if (!s->tex->uploaded) {
                                upload_texture(r, s->tex);
                        }
                        switchTexture(r, s->tex);
                        draw_buffers(r, vert_sb, tex_coord_sb);
                        sb_reset(vert_sb);
                        sb_reset(tex_coord_sb);
                }
        }

        sb_free(vert_sb);
        sb_free(tex_coord_sb);

        glUseProgram(0);
}

float* calc_verts(sprite* s, float* vert_buffer)
{
        float tex_width = s->tex_rect.w == 0.0f ? s->tex->width : s->tex_rect.w;
        float width = tex_width * s->scale;

        float tex_height = s->tex_rect.h == 0.0f ? s->tex->height : s->tex_rect.h;
        float height = tex_height * s->scale;

        kmVec2 bl; // bottom left
        bl.x = s->x_pos;
        bl.y = s->y_pos;

        kmVec2 br; // bottom right
        br.x = s->x_pos + width;
        br.y = s->y_pos;

        kmVec2 tl; // top left
        tl.x = s->x_pos;
        tl.y = s->y_pos + height;

        kmVec2 tr; // top right
        tr.x = s->x_pos + width;
        tr.y = s->y_pos + height;

        if (s->rotation != 0.0f) {
                kmVec2 anchor;
                anchor.x = s->x_anchor;
                anchor.y = s->y_anchor;
                kmVec2RotateBy(&bl, &bl, s->rotation, &anchor);
                kmVec2RotateBy(&br, &br, s->rotation, &anchor);
                kmVec2RotateBy(&tl, &tl, s->rotation, &anchor);
                kmVec2RotateBy(&tr, &tr, s->rotation, &anchor);
        }
        sb_push(vert_buffer, bl.x);
        sb_push(vert_buffer, bl.y);
        sb_push(vert_buffer, tl.x);
        sb_push(vert_buffer, tl.y);
        sb_push(vert_buffer, tr.x);
        sb_push(vert_buffer, tr.y);
        sb_push(vert_buffer, bl.x);
        sb_push(vert_buffer, bl.y);
        sb_push(vert_buffer, tr.x);
        sb_push(vert_buffer, tr.y);
        sb_push(vert_buffer, br.x);
        sb_push(vert_buffer, br.y);

        return vert_buffer;
}

float* calc_tex_coords(sprite* s, float* tex_coord_buffer)
{
        float tex_width = (float)s->tex->width;
        float tex_height = (float)s->tex->height;
        float x = s->tex_rect.x;
        float y = s->tex_rect.y;
        float w = s->tex_rect.w == 0.0f ? tex_width : s->tex_rect.w;
        float h = s->tex_rect.h == 0.0f ? tex_height : s->tex_rect.h;

        float tex_bot = (y + h) / tex_height;
        float tex_top = y / tex_height;
        float tex_left = s->flip_x ? ((x + w) / tex_width) : (x / tex_width);
        float tex_right = s->flip_x ? (x / tex_width) : ((x + w) / tex_width);

        sb_push(tex_coord_buffer, tex_left);
        sb_push(tex_coord_buffer, tex_bot);
        sb_push(tex_coord_buffer, tex_left);
        sb_push(tex_coord_buffer, tex_top);
        sb_push(tex_coord_buffer, tex_right);
        sb_push(tex_coord_buffer, tex_top);
        sb_push(tex_coord_buffer, tex_left);
        sb_push(tex_coord_buffer, tex_bot);
        sb_push(tex_coord_buffer, tex_right);
        sb_push(tex_coord_buffer, tex_top);
        sb_push(tex_coord_buffer, tex_right);
        sb_push(tex_coord_buffer, tex_bot);

        return tex_coord_buffer;
}

void draw_buffers(renderer* r, float* vert_sb, float* tex_coord_sb)
{
        int vert_count = sb_count(vert_sb);
        uint32_t vert_data_size = vert_count * sizeof(float);
        GLuint gl_vert_buffer = make_buffer(GL_ARRAY_BUFFER,
                                            vert_sb, vert_data_size);

        glBindBuffer(GL_ARRAY_BUFFER, gl_vert_buffer);
        glEnableVertexAttribArray(r->vert_attrib);
        glVertexAttribPointer(r->vert_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        uint32_t tex_data_size = sb_count(tex_coord_sb) * sizeof(float);
        GLuint gl_tex_buffer = make_buffer(GL_ARRAY_BUFFER,
                                           tex_coord_sb, tex_data_size);

        glBindBuffer(GL_ARRAY_BUFFER, gl_tex_buffer);
        glEnableVertexAttribArray(r->tex_coord_attrib);
        glVertexAttribPointer(r->tex_coord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, vert_count / 2);

        glDeleteBuffers(1, &gl_vert_buffer);
        glDeleteBuffers(1, &gl_tex_buffer);
        glDisableVertexAttribArray(r->vert_attrib);
        glDisableVertexAttribArray(r->tex_coord_attrib);
}

bool upload_texture(renderer* r, texture* t)
{
        assert(t);

        if (t->uploaded) {
                return true;
        }

        glGenTextures(1, &t->gl_id);
        glBindTexture(GL_TEXTURE_2D, t->gl_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t->width, t->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, t->data);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (check_gl_error()) {
                LOGERR("%s", "A GL error occurred when loading texture");
                return false;
        }

        t->uploaded = true;

        return true;
}