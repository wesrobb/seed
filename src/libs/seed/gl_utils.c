#include "gl_utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "file_utils.h"
#include "log.h"

static void show_info_log(GLuint object, PFNGLGETSHADERIVPROC glGet__iv,
                          PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
        GLint log_length;
        char *log;

        glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
        log = malloc(log_length);
        glGet__InfoLog(object, log_length, NULL, log);
        LOGERR("%s", log);
        free(log);
}

GLuint make_shader(GLenum type, const char *filename)
{
        GLint length;
        GLchar *source = (GLchar*)file_contents(filename, &length);
        GLuint shader;
        GLint shader_ok;

        if (!source)
                return 0;

        shader = glCreateShader(type);
        glShaderSource(shader, 1, (const GLchar**)&source, &length);
        free(source);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
        if (!shader_ok) {
                LOGERR("Failed to compile %s:", filename);
                show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
                glDeleteShader(shader);
                return 0;
        }
        return shader;
}

GLuint make_program(GLuint vertex_shader, GLuint fragment_shader)
{
        GLint program_ok;

        GLuint program = glCreateProgram();

        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
        if (!program_ok) {
                LOGERR("%s", "Failed to link shader program:");
                show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
                glDeleteProgram(program);
                return 0;
        }
        return program;
}

GLuint make_buffer(GLenum target, const void *buffer_data, GLsizei buffer_size)
{
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(target, buffer);
        glBufferData(target, buffer_size, buffer_data, GL_STREAM_DRAW);
        return buffer;
}

void update_buffer(GLenum target, GLuint buffer_object,
                   const void *buffer_data, GLsizei buffer_size)
{
        glBindBuffer(target, buffer_object);
        glBufferSubData(target, 0, buffer_size, buffer_data);
}
