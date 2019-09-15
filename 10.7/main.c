#include <stddef.h>
#include <epoxy/gl.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <shader_make.h>
#include <glmath.h>

typedef struct {
	vec3 position;
	vec2 texture;
} vertex;

static vertex vertices[] = {
	{{+0.5f, +0.5f, +0.5f}, {0.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}},
	{{+0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}},
	{{+0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
	{{+0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}},

	{{+0.5f, +0.5f, +0.5f}, {0.0f, 1.0f}},
	{{+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}},
	{{-0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}},
	{{+0.5f, +0.5f, +0.5f}, {0.0f, 1.0f}},
	{{-0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f}},

	{{-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, 1.0f}},
	{{-0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}},
	{{-0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},

	{{+0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
	{{+0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},

	{{+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}},
	{{+0.5f, +0.5f, +0.5f}, {1.0f, 0.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}},

	{{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}},
	{{+0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
	{{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
	{{-0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}}
};

static const vec3 cubePositions[] = {
	{0.0f, 0.0f, 0.0f},
	{2.0f, 5.0f, -15.0f},
	{-1.5f, -2.2f, -2.5f},
	{-3.8f, -2.0f, -12.3f},
	{2.4f, -0.4f, -3.5f},
	{-1.7f, 3.0f, -7.5f},
	{1.3f, -2.0f, -2.5f},
	{1.5f, 2.0f, -2.5f},
	{1.5f, 0.2f, -1.5f},
	{-1.3f, 1.0f, -1.5f}
};

static GLuint vao;
static GLuint vbo;
static GLuint program;

static GLuint texture[2];

static const char *const filename[2] = {
	"container.jpg",
	"awesomeface.png"
};

static vec3 cameraPos = {0.0f, 0.0f, 3.0f};
static vec3 cameraFront = {0.0f, 0.0f, -1.0f};
static vec3 cameraUp = {0.0f, 1.0f, 0.0f};

static gdouble yaw = -90., pitch = 0;
double lastX = 0, lastY = 0;

static GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
static GLfloat lastFrame = 0.0f;	// Time of last frame

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

	for (unsigned int i = 0; i < G_N_ELEMENTS(filename); ++i) {
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
		const GLint format = gdk_pixbuf_get_has_alpha(pixbuf) ? GL_RGBA : GL_RGB;
		const guint8 *bytes = gdk_pixbuf_read_pixels(pixbuf);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, bytes);
		glGenerateMipmap(GL_TEXTURE_2D);
		g_object_unref(G_OBJECT(pixbuf));
	}

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, texture));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glUseProgram(program);

	glUniform1i(glGetUniformLocation(program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(program, "texture2"), 1);

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
	const mat4 view = mat4_look_at(cameraPos, vec3_add(cameraPos, cameraFront), cameraUp);

	const GLint width = gtk_widget_get_allocated_width(GTK_WIDGET(area));
	const GLint height = gtk_widget_get_allocated_height(GTK_WIDGET(area));
	const mat4 projection = mat4_perspective(to_radians(45.), ((GLfloat) width) / ((GLfloat) height), 1., 100.);

	glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (const GLfloat *) &view);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (const GLfloat *) &projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindVertexArray(vao);

	for (unsigned int i = 0; i < G_N_ELEMENTS(cubePositions); i++) {
		const mat4 model = mat4_mul(mat4_translation(cubePositions[i]), mat4_rotation(to_radians(20.f * i), (vec3) {1.0f, 0.3f, 0.5f}));

		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const GLfloat *) &model);
		glDrawArrays(GL_TRIANGLES, 0, G_N_ELEMENTS(vertices));
	}

	glBindVertexArray(0);
	glUseProgram(0);

	return TRUE;
}

static gboolean key_press_event(GtkGLArea *drawing, GdkEvent *event, gpointer user_data)
{
	if (gdk_event_get_event_type(event) != GDK_KEY_PRESS) {
		return FALSE;
	}

	guint keyval;

	if (gdk_event_get_keyval(event, &keyval) == FALSE) {
		return FALSE;
	}

	const GLfloat cameraSpeed = 2.5 * deltaTime;

	switch (keyval) {
	case 'W':
	case 'w':
		cameraPos = vec3_add(cameraPos, vec3_mulf(cameraFront, cameraSpeed));
		break;
	case 'S':
	case 's':
		cameraPos = vec3_sub(cameraPos, vec3_mulf(cameraFront, cameraSpeed));
		break;
	case 'A':
	case 'a':
		cameraPos = vec3_sub(cameraPos, vec3_mulf(vec3_normalize(vec3_cross(cameraFront, cameraUp)), cameraSpeed));
		break;
	case 'D':
	case 'd':
		cameraPos = vec3_add(cameraPos, vec3_mulf(vec3_normalize(vec3_cross(cameraFront, cameraUp)), cameraSpeed));
		break;
	}

	gtk_gl_area_queue_render(drawing);

	return TRUE;
}

static int ontick(GtkWidget *area, GdkFrameClock *frame_clock, gpointer user_data)
{
	GLfloat currentFrame = g_timer_elapsed(timer, NULL);

	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	gtk_gl_area_queue_render(GTK_GL_AREA(area));

	return G_SOURCE_CONTINUE;
}

static gboolean enter_notify_event(GtkGLArea *drawing, GdkEvent *event, gpointer user_data)
{
	if (gdk_event_get_event_type(event) != GDK_ENTER_NOTIFY) {
		return FALSE;
	}

	gdouble xpos, ypos;

	gdk_event_get_coords(event, &xpos, &ypos);
	lastX = xpos;
	lastY = ypos;

	return TRUE;
}

static gboolean motion_notify_event(GtkGLArea *drawing, GdkEvent *event, gpointer user_data)
{
	if (gdk_event_get_event_type(event) != GDK_MOTION_NOTIFY) {
		return FALSE;
	}

	gdouble xpos, ypos;

	gdk_event_get_coords(event, &xpos, &ypos);

	const gdouble xoffset = -(xpos - lastX);
	const gdouble yoffset = -(lastY - ypos);
	const gdouble sensitivity = 0.05f;

	lastX = xpos;
	lastY = ypos;

	yaw += xoffset * sensitivity;
	pitch += yoffset * sensitivity;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	const vec3 front = (vec3) {
		.x = cos(to_radians(pitch)) * cos(to_radians(yaw)),
		.y = sin(to_radians(pitch)),
		.z = cos(to_radians(pitch)) * sin(to_radians(yaw))
	};
	cameraFront = vec3_normalize(front);

	gtk_gl_area_queue_render(drawing);

	return TRUE;
}

static void activate(GtkApplication *application, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *drawing;

	drawing = gtk_gl_area_new();
	gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(drawing), TRUE);
	g_signal_connect(G_OBJECT(drawing), "realize", G_CALLBACK(realize), NULL);
	g_signal_connect(G_OBJECT(drawing), "unrealize", G_CALLBACK(unrealize), NULL);
	g_signal_connect(G_OBJECT(drawing), "render", G_CALLBACK(render), NULL);
	g_signal_connect(G_OBJECT(drawing), "enter-notify-event", G_CALLBACK(enter_notify_event), NULL);
	g_signal_connect(G_OBJECT(drawing), "motion-notify-event", G_CALLBACK(motion_notify_event), NULL);
	g_signal_connect(G_OBJECT(drawing), "key-press-event", G_CALLBACK(key_press_event), NULL);
	gtk_widget_add_events(drawing, GDK_ENTER_NOTIFY_MASK | GDK_POINTER_MOTION_MASK);
	gtk_widget_add_tick_callback(drawing, ontick, NULL, NULL);
	gtk_widget_set_can_focus(drawing, TRUE);

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
