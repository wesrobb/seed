#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glew/glew.h>
#include <log/log.h>

#include "camera.h"
#include "gl_utils.h"
#include "khash.h"
#include "sprite.h"
#include "texture.h"

#define CHECG_GL

#define MAX_SPRITES 5000
static uint32_t _num_sprites;

static GLuint _shader_prog;
static GLuint _vertex_attrib;
static GLuint _tex_coord_attrib;

static uint32_t _last_texture_id;
static uint32_t _tex_unit;

// 6 points per quad, 2 floats per point.
// The buffers are passed to openGL.
static float _vert_data[MAX_SPRITES * 6 * 2];
static float _tex_data[MAX_SPRITES * 6 * 2];
static uint32_t _vert_index;
static uint32_t _tex_index;

static uint32_t _virtual_width;
static uint32_t _virtual_height;
static uint32_t _screen_width;
static uint32_t _screen_height;

void resetBuffers()
{
        _num_sprites = 0;
        _vert_index = 0;
        _tex_index = 0;
}

void bindTextureUnit(uint32_t shader_prog, 
                     uint32_t tex_unit, 
                     const char* uniform_name)
{
        GLuint tex_uniform = glGetUniformLocation(shader_prog, uniform_name);
        glUseProgram(shader_prog);
        glUniform1i(tex_uniform, tex_unit);
        glUseProgram(0);
}

void switchTexture(Texture* t)
{
        assert(t);

        glActiveTexture(GL_TEXTURE0 + _tex_unit);
        glBindTexture(GL_TEXTURE_2D, t->id);
        _last_texture_id = t->id;
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

bool checkGLError()
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

bool Render_init(uint32_t screen_width, uint32_t screen_height,
                 uint32_t virtual_width, uint32_t virtual_height,
                 const char* vert_shader_path, const char* frag_shader_path)
{
        GLuint vert_shader = make_shader(GL_VERTEX_SHADER, vert_shader_path);
        if (vert_shader == 0) {
                LOGERR("%s", "Error making vertex shader");
                goto return_failed;
        }

        GLuint frag_shader = make_shader(GL_FRAGMENT_SHADER, frag_shader_path);
        if (frag_shader == 0) {
                LOGERR("%s", "Error making fragment shader");
                goto cleanup_vert;
        }

        _shader_prog = make_program(vert_shader, frag_shader);
        if (_shader_prog == 0) {
                LOGERR("%s", "Error making shader program");
                goto cleanup_frag;
        }

        _vertex_attrib = glGetAttribLocation(_shader_prog, "vertex");
        _tex_coord_attrib = glGetAttribLocation(_shader_prog, "tex_coord");
        _virtual_width = virtual_width;
        _virtual_height = virtual_height;
        _last_texture_id = -1;
        _tex_unit = 0;

        resetBuffers();
        bindTextureUnit(_shader_prog, _tex_unit, "sprite_texture");
        bindSampler(_tex_unit);
        Render_resize(screen_width, screen_height);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        if (checkGLError()) {
                goto cleanup_shader_prog;
        }

        return true;

cleanup_shader_prog:
        glDeleteProgram(_shader_prog);
cleanup_frag:
        glDeleteShader(frag_shader);
cleanup_vert:
        glDeleteShader(vert_shader);
return_failed:
        return false;
}

void Render_free()
{
        glDeleteProgram(_shader_prog);
}

void Render_resize(uint32_t screen_width, uint32_t screen_height)
{
        _screen_width = screen_width;
        _screen_height = screen_height;
        float aspect_ratio = ((float)_virtual_width) / ((float)_virtual_height);
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

        GLint proj_uniform = glGetUniformLocation(_shader_prog, "projection");
        kmMat4 proj_matrix;
        kmMat4OrthographicProjection(&proj_matrix,
                                     0, (float)_virtual_width,
                                     0, (float)_virtual_height,
                                     -1, 1);
        glUseProgram(_shader_prog);
        glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, proj_matrix.mat);
        glUseProgram(0);

        if (checkGLError()) {
                LOGERR("%s", "An GL error occurred when resizing renderer");
        }
}

void Render_addSprite(const Sprite* s)
{
        assert(s);

        if (s->texture->id != _last_texture_id ||
            _num_sprites >= MAX_SPRITES) {
                Render_end(); // Not really ending, just flushing the renderer.
                switchTexture(s->texture);
        }

        _num_sprites++;

        kmVec2 bl; // bottom left
        bl.x = s->x_pos;
        bl.y = s->y_pos;

        kmVec2 br; // bottom right
        br.x = s->x_pos + s->width;
        br.y = s->y_pos;

        kmVec2 tl; // top left
        tl.x = s->x_pos;
        tl.y = s->y_pos + s->height;

        kmVec2 tr; // top right
        tr.x = s->x_pos + s->width;
        tr.y = s->y_pos + s->height;

        if (s->rotation != 0.0f)
        {
                kmVec2 anchor;
                anchor.x = s->x_anchor;
                anchor.y = s->y_anchor;
                kmVec2RotateBy(&bl, &bl, s->rotation, &anchor);
                kmVec2RotateBy(&br, &br, s->rotation, &anchor);
                kmVec2RotateBy(&tl, &tl, s->rotation, &anchor);
                kmVec2RotateBy(&tr, &tr, s->rotation, &anchor); 
        }

        _vert_data[_vert_index++] = bl.x;
        _vert_data[_vert_index++] = bl.y;
        _vert_data[_vert_index++] = tl.x;
        _vert_data[_vert_index++] = tl.y;
        _vert_data[_vert_index++] = tr.x;
        _vert_data[_vert_index++] = tr.y;
        _vert_data[_vert_index++] = bl.x;
        _vert_data[_vert_index++] = bl.y;
        _vert_data[_vert_index++] = tr.x;
        _vert_data[_vert_index++] = tr.y;
        _vert_data[_vert_index++] = br.x;
        _vert_data[_vert_index++] = br.y;

        // Texture coordinates indices.
        /*uint32_t bli = 0;
        uint32_t tli = 2;
        uint32_t tri = 4;
        uint32_t bri = 6;
        _tex_data[_tex_index++] = s->tex_coords[bli];
        _tex_data[_tex_index++] = s->tex_coords[bli + 1];
        _tex_data[_tex_index++] = s->tex_coords[tli];
        _tex_data[_tex_index++] = s->tex_coords[tli + 1];
        _tex_data[_tex_index++] = s->tex_coords[tri];
        _tex_data[_tex_index++] = s->tex_coords[tri + 1];
        _tex_data[_tex_index++] = s->tex_coords[bli];
        _tex_data[_tex_index++] = s->tex_coords[bli + 1];
        _tex_data[_tex_index++] = s->tex_coords[tri];
        _tex_data[_tex_index++] = s->tex_coords[tri + 1];
        _tex_data[_tex_index++] = s->tex_coords[bri];
        _tex_data[_tex_index++] = s->tex_coords[bri + 1];*/
        uint32_t bli = 0;
        uint32_t tli = 2;
        uint32_t tri = 4;
        uint32_t bri = 6;
        _tex_data[_tex_index++] = s->tex_coords[bli];
        _tex_data[_tex_index++] = s->tex_coords[bli + 1];
        _tex_data[_tex_index++] = s->tex_coords[tli];
        _tex_data[_tex_index++] = s->tex_coords[tli + 1];
        _tex_data[_tex_index++] = s->tex_coords[tri];
        _tex_data[_tex_index++] = s->tex_coords[tri + 1];
        _tex_data[_tex_index++] = s->tex_coords[bli];
        _tex_data[_tex_index++] = s->tex_coords[bli + 1];
        _tex_data[_tex_index++] = s->tex_coords[tri];
        _tex_data[_tex_index++] = s->tex_coords[tri + 1];
        _tex_data[_tex_index++] = s->tex_coords[bri];
        _tex_data[_tex_index++] = s->tex_coords[bri + 1];
}

uint32_t Render_loadTexture(unsigned char* data,
                            uint32_t width, uint32_t height,
                            uint32_t channels)
{
        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (checkGLError()) {
                LOGERR("%s", "An GL error occurred when loading texture");
                return -1;
        }

        return texture_id;
}

void Render_begin(void)
{
        glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render_end(void)
{
        glUseProgram(_shader_prog);

        // Set the camera transfrom
        GLint cam_uniform = glGetUniformLocation(_shader_prog, "cam");
        kmMat4* cam_mat = Cam_getTransform();
        glUniformMatrix4fv(cam_uniform, 1, GL_FALSE, cam_mat->mat);

        uint32_t vert_data_size = _num_sprites * 6 * 2 * sizeof(float);
        GLuint gl_vert_buffer = make_buffer(GL_ARRAY_BUFFER,
                                            _vert_data, vert_data_size);

        glBindBuffer(GL_ARRAY_BUFFER, gl_vert_buffer);
        glEnableVertexAttribArray(_vertex_attrib);
        glVertexAttribPointer(_vertex_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        uint32_t tex_data_size = _num_sprites * 6 * 2 * sizeof(float);
        GLuint gl_tex_buffer = make_buffer(GL_ARRAY_BUFFER,
                                           _tex_data, tex_data_size);

        glBindBuffer(GL_ARRAY_BUFFER, gl_tex_buffer);
        glEnableVertexAttribArray(_tex_coord_attrib);
        glVertexAttribPointer(_tex_coord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, _num_sprites * 6);

        glDeleteBuffers(1, &gl_vert_buffer);
        glDeleteBuffers(1, &gl_tex_buffer);
        glDisableVertexAttribArray(_vertex_attrib);
        glDisableVertexAttribArray(_tex_coord_attrib);
        glUseProgram(0);


        if (checkGLError()) {
                LOGERR("%s", "An GL error occurred when flushing renderer.");
        }

        resetBuffers();
}
