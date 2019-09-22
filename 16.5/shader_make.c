#include <glib.h>
#include <shader_compile.h>
#include <shader_make.h>

GLuint shader_make()
{
	GLuint vertex, fragment;
	GLuint program;
	GLint success;

	// container
	extern const GLchar _binary____shader_container_vertex_glsl_start;
	extern const GLchar _binary____shader_container_vertex_glsl_end;
	extern const GLchar _binary____shader_container_fragment_glsl_start;
	extern const GLchar _binary____shader_container_fragment_glsl_end;

	vertex = shader_compile(GL_VERTEX_SHADER, &_binary____shader_container_vertex_glsl_start, &_binary____shader_container_vertex_glsl_end - &_binary____shader_container_vertex_glsl_start);
	fragment = shader_compile(GL_FRAGMENT_SHADER, &_binary____shader_container_fragment_glsl_start, &_binary____shader_container_fragment_glsl_end - &_binary____shader_container_fragment_glsl_start);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLchar message[512];

		glGetProgramInfoLog(program, sizeof message, NULL, message);
		g_error("Link error: %s\n", message);
		program = 0;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}
