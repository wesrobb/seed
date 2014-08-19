#pragma once

#include <glew/glew.h>

GLuint make_shader(GLenum type, const char *filename);
GLuint make_program(GLuint vertex_shader, GLuint fragment_shader);
GLuint make_buffer(GLenum target, const void *buffer_data, GLsizei buffer_size);
void update_buffer(GLenum target, GLuint buffer_object, 
                   const void *buffer_data, GLsizei buffer_size);
