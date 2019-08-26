#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <shader_make.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glmath.h>

static GLfloat vertices[] = {
	// positions // texture coords
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f,

	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f,

	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

	0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

	-0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 0.0f
};

static GLuint vao;
static GLuint vbo;
static GLuint program;

static GLuint texture[2];

static const char *const filename[2] = {
	"container.jpg",
	"awesomeface.png"
};

GTimer *timer;

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

	glGenTextures(2, texture);

	for (int i = 0; i < G_N_ELEMENTS(filename); ++i) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GError *error = NULL;
		GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename[i], &error);

		if (pixbuf == NULL) {
			g_error("Error loading file: #%d %s\n", error->code, error->message);
			g_error_free(error);
		}
		const GLint width = gdk_pixbuf_get_width(pixbuf);
		const GLint height = gdk_pixbuf_get_height(pixbuf);
		const GLint type = gdk_pixbuf_get_has_alpha(pixbuf) ? GL_RGBA : GL_RGB;
		const guint8 *bytes = gdk_pixbuf_read_pixels(pixbuf);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, type, GL_UNSIGNED_BYTE, bytes);
		glGenerateMipmap(GL_TEXTURE_2D);
		g_object_unref(G_OBJECT(pixbuf));
	}

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), (void *) (3 * sizeof (GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glUseProgram(program);

	glUniform1i(glGetUniformLocation(program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(program, "texture2"), 1);

	const mat4 view = mat4_translation((vec3) { 0.0f, 0.0f, -3.0f });

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (const GLfloat *) &view);

	glUseProgram(0);
}

static void unrealize(GtkGLArea *area, gpointer user_data)
{
	gtk_gl_area_make_current(area);
	if (gtk_gl_area_get_error(area) != NULL) {
		return;
	}

	glDeleteTextures(2, texture);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);

	g_timer_destroy(timer);
}

static gboolean render(GtkGLArea *area, GdkGLContext *context, gpointer user_data)
{
	const GLint width = gtk_widget_get_allocated_width(GTK_WIDGET(area));
	const GLint height = gtk_widget_get_allocated_height(GTK_WIDGET(area));
	const mat4 model = mat4_rotation(g_timer_elapsed(timer, NULL) * to_radians(50.f), (vec3) { 0.5f, 1.0f, 0.0f });
	const mat4 projection = mat4_perspective(to_radians(45.), ((GLfloat) width) / ((GLfloat) height), 1., 100.);

	glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const GLfloat *) &model);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (const GLfloat *) &projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);	// G_N_ELEMENTS(vertices) / 5);
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
