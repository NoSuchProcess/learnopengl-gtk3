#include <stddef.h>
#include <epoxy/gl.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <shader_make.h>
#include <glmath.h>

typedef struct {
	vec2 position;
} vertex;

static vertex vertices1[] = {
	{{-0.5f, -0.25f}},
	{{0.0f, -0.25f}},
	{{-0.25f, +0.25f}}
};

static vertex vertices2[] = {
	{{0.0f, -0.25f}},
	{{+0.5f, -0.25f}},
	{{+0.25f, +0.25f}}
};

static GLuint vao[2];
static GLuint vbo[2];
static GLuint program;

static void realize(GtkGLArea *area, gpointer user_data)
{
	gtk_gl_area_make_current(area);
	if (gtk_gl_area_get_error(area) != NULL) {
		return;
	}

	program = shader_make();

	{
		GLint index;

		glGenVertexArrays(2, vao);
		glBindVertexArray(vao[0]);

		glGenBuffers(2, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices1, vertices1, GL_STATIC_DRAW);

		index = glGetAttribLocation(program, "position");
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, position));
		glEnableVertexAttribArray(index);

		glBindVertexArray(vao[1]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices2, vertices2, GL_STATIC_DRAW);

		index = glGetAttribLocation(program, "position");
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, position));
		glEnableVertexAttribArray(index);

		glBindVertexArray(0);
	}
}

static void unrealize(GtkGLArea *area, gpointer user_data)
{
	gtk_gl_area_make_current(area);
	if (gtk_gl_area_get_error(area) != NULL) {
		return;
	}

	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
	glDeleteProgram(program);
}

static gboolean render(GtkGLArea *area, GdkGLContext *context, gpointer user_data)
{
	glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, G_N_ELEMENTS(vertices1));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, G_N_ELEMENTS(vertices2));
	glBindVertexArray(0);
	glUseProgram(0);

	return TRUE;
}

static void activate(GtkApplication *application, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *drawing;

	drawing = gtk_gl_area_new();
	g_signal_connect(G_OBJECT(drawing), "realize", G_CALLBACK(realize), NULL);
	g_signal_connect(G_OBJECT(drawing), "unrealize", G_CALLBACK(unrealize), NULL);
	g_signal_connect(G_OBJECT(drawing), "render", G_CALLBACK(render), NULL);

	window = gtk_application_window_new(application);
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_container_add(GTK_CONTAINER(window), drawing);

	gtk_widget_show_all(window);
}

int main(int argc, char *argv[])
{
	int result;
	GtkApplication *application;

	application = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(G_OBJECT(application), "activate", G_CALLBACK(activate), NULL);
	result = g_application_run(G_APPLICATION(application), argc, argv);
	g_object_unref(G_OBJECT(application));

	return result;
}
