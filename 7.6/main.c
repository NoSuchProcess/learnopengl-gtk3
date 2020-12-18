#include <stddef.h>
#include <epoxy/gl.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <shader_make.h>
#include <glmath.h>

typedef struct {
	vec2 position;
	vec3 color;
	vec2 texture;
} vertex;

static vertex vertices[] = {
	{{+0.5f, +0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},	// top right
	{{+0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},	// bottom right
	{{-0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},	// bottom left
	{{-0.5f, +0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}	// top left
};

static GLuint indices[] = {
	0, 1, 3,		// first triangle
	1, 2, 3			// second triangle
};

static GLuint vao;
static GLuint vbo;
static GLuint ebo;
static GLuint texture;
static GLuint program;

static void realize(GtkGLArea *area, gpointer user_data)
{
	GdkPixbuf *pixbuf;
	GLint width, height;
	GError *error = NULL;

	gtk_gl_area_make_current(area);
	if (gtk_gl_area_get_error(area) != NULL) {
		return;
	}

	program = shader_make();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	pixbuf = gdk_pixbuf_new_from_file("container.jpg", &error);
	if (pixbuf == NULL) {
		g_error("Error loading file: #%d %s\n", error->code, error->message);
		g_error_free(error);
	}
	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_height(pixbuf);
	const guint8 *bytes = gdk_pixbuf_read_pixels(pixbuf);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	g_object_unref(G_OBJECT(pixbuf));

	{
		GLint index;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

		index = glGetAttribLocation(program, "position");
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, position));
		glEnableVertexAttribArray(index);
		index = glGetAttribLocation(program, "color");
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, color));
		glEnableVertexAttribArray(index);
		index = glGetAttribLocation(program, "tex_coord");
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, texture));
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

	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteProgram(program);
}

static gboolean render(GtkGLArea *area, GdkGLContext *context, gpointer user_data)
{
	glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, G_N_ELEMENTS(indices), GL_UNSIGNED_INT, 0);
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
	gtk_window_set_child(GTK_WINDOW(window), drawing);

	gtk_widget_show(window);
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
