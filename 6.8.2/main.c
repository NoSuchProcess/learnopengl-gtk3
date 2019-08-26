#include <math.h>
#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <shader_make.h>

static GLfloat vertices[] = {
	// positions // colors
	+0.5f, -0.5f, 1.0f, 0.0f, 0.0f,	// bottom right
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// bottom left
	+0.0f, +0.5f, 0.0f, 0.0f, 1.0f	// top
};

static GLuint vao;
static GLuint vbo;
static GLuint program;

static GTimer *timer;

static void realize(GtkGLArea *area, gpointer user_data)
{
	gtk_gl_area_make_current(area);
	if (gtk_gl_area_get_error(area) != NULL) {
		return;
	}

	timer = g_timer_new();

	program = shader_make();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (void *) (2 * sizeof (GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

static void unrealize(GtkGLArea *area, gpointer user_data)
{
	gtk_gl_area_make_current(area);
	if (gtk_gl_area_get_error(area) != NULL) {
		return;
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);

	g_timer_destroy(timer);
}

static gboolean render(GtkGLArea *area, GdkGLContext *context, gpointer user_data)
{
	const GLfloat offset = sin(g_timer_elapsed(timer, NULL)) / 2.;
	GLint location;

	glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	location = glGetUniformLocation(program, "offset");

	glUseProgram(program);
	glUniform1f(location, offset);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, G_N_ELEMENTS(vertices) / 5);
	glBindVertexArray(0);
	glUseProgram(0);

	return TRUE;
}

static gboolean ontick(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer user_data)
{
	gtk_gl_area_queue_render(GTK_GL_AREA(widget));

	return G_SOURCE_CONTINUE;
}

static void activate(GtkApplication *application, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *drawing;

	drawing = gtk_gl_area_new();
	g_signal_connect(G_OBJECT(drawing), "realize", G_CALLBACK(realize), NULL);
	g_signal_connect(G_OBJECT(drawing), "unrealize", G_CALLBACK(unrealize), NULL);
	g_signal_connect(G_OBJECT(drawing), "render", G_CALLBACK(render), NULL);
	gtk_widget_add_tick_callback(drawing, ontick, NULL, NULL);

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
