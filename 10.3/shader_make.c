#include <glib.h>
#include <shader_compile.h>
#include <shader_make.h>

GLuint shader_make()
{
	GLuint vertex, fragment;
	GLuint program;
	GLint success;

	extern const GLchar _binary____10_3_shader_shader_vert_start;
	extern const GLchar _binary____10_3_shader_shader_vert_end;
	extern const GLchar _binary____10_3_shader_shader_frag_start;
	extern const GLchar _binary____10_3_shader_shader_frag_end;

	vertex = shader_compile(GL_VERTEX_SHADER, &_binary____10_3_shader_shader_vert_start, &_binary____10_3_shader_shader_vert_end - &_binary____10_3_shader_shader_vert_start);
	fragment = shader_compile(GL_FRAGMENT_SHADER, &_binary____10_3_shader_shader_frag_start, &_binary____10_3_shader_shader_frag_end - &_binary____10_3_shader_shader_frag_start);

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
