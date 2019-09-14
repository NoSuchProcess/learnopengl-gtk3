#include <stddef.h>
#include <epoxy/gl.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <shader_make.h>
#include <glmath.h>

typedef struct {
	vec3 position;
	vec3 normal;
	vec2 texture;
} vertix;

static vertix vertices[] = {
	// positions         // normals           // texture
	{{+0.5f, +0.5f, +0.5f}, {+1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {+1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{+0.5f, +0.5f, -0.5f}, {+1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {+1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{+0.5f, -0.5f, -0.5f}, {+1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{+0.5f, +0.5f, -0.5f}, {+1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

	{{+0.5f, +0.5f, +0.5f}, {0.0f, +1.0f, 0.0f}, {0.0f, 1.0f}},
	{{+0.5f, +0.5f, -0.5f}, {0.0f, +1.0f, 0.0f}, {1.0f, 1.0f}},
	{{-0.5f, +0.5f, -0.5f}, {0.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},
	{{+0.5f, +0.5f, +0.5f}, {0.0f, +1.0f, 0.0f}, {0.0f, 1.0f}},
	{{-0.5f, +0.5f, -0.5f}, {0.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, +1.0f, 0.0f}, {0.0f, 0.0f}},

	{{-0.5f, -0.5f, +0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{-0.5f, +0.5f, +0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{-0.5f, +0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f, +0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{-0.5f, +0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

	{{+0.5f, -0.5f, +0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
	{{+0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f, +0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},

	{{+0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {1.0f, 1.0f}},
	{{+0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {1.0f, 0.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {0.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {1.0f, 1.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {0.0f, 1.0f}},

	{{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
	{{+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
	{{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
	{{-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}}
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
	{-1.3f, 1.0f, -1.5f},
};

static GLuint vao;
static GLuint vbo;
static GLuint program;
static GLuint texture[2];

static vec3 cameraPos = {0.0f, 0.0f, 5.0f};
static vec3 cameraFront = {0.0f, 0.0f, -1.0f};
static vec3 cameraUp = {0.0f, 1.0f, 0.0f};

#define FOV_MAX 60.
#define FOV_MIN 1.

static gdouble fov = 45., yaw = -90., pitch = 0;
double lastX = 0, lastY = 0;

static void realize(GtkGLArea *area, gpointer user_data)
{
	gtk_gl_area_make_current(area);
	if (gtk_gl_area_get_error(area) != NULL) {
		return;
	}

	program = shader_make();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (vertix), (const void *) offsetof(vertix, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (vertix), (const void *) offsetof(vertix, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof (vertix), (const void *) offsetof(vertix, texture));
	glEnableVertexAttribArray(2);

	glGenTextures(2, texture);
	for (unsigned int i = 0; i < G_N_ELEMENTS(texture); ++i) {
		static const char *const filename[G_N_ELEMENTS(texture)] = {
			"container.png",
			"container_specular.png",
		};
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

	glBindVertexArray(0);
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
}

static gboolean render(GtkGLArea *area, GdkGLContext *context, gpointer user_data)
{
	mat4 model;
	const mat4 view = mat4_look_at(cameraPos, vec3_add(cameraPos, cameraFront), cameraUp);
	const GLint width = gtk_widget_get_allocated_width(GTK_WIDGET(area));
	const GLint height = gtk_widget_get_allocated_height(GTK_WIDGET(area));
	const mat4 projection = mat4_perspective(to_radians(fov), ((GLfloat) width) / ((GLfloat) height), 1., 100.);

	glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Container
	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, (const GLfloat *) &cameraPos);

	glUniform1i(glGetUniformLocation(program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(program, "material.specular"), 1);
	glUniform1f(glGetUniformLocation(program, "material.shininess"), 32.0f);

	glUniform3fv(glGetUniformLocation(program, "light.position"), 1,  (const GLfloat *) &cameraPos);
	glUniform3fv(glGetUniformLocation(program, "light.direction"), 1, (const GLfloat *) &cameraFront);
	glUniform1f(glGetUniformLocation(program, "light.cutOff"), cos(to_radians(12.5f)));
	glUniform1f(glGetUniformLocation(program, "light.outerCutOff"), cos(to_radians(17.5f)));

	glUniform3f(glGetUniformLocation(program, "light.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(program, "light.diffuse"), 0.5f, 0.5f, 0.5f);	// darken the light a bit to fit the scene
	glUniform3f(glGetUniformLocation(program, "light.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(program, "light.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "light.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(program, "light.quadratic"), 0.032f);

	model = mat4_identity();

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const GLfloat *) &model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (const GLfloat *) &view);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (const GLfloat *) &projection);

	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	for (unsigned int i = 0; i < G_N_ELEMENTS(cubePositions); i++) {
		const mat4 model = mat4_mul(mat4_translation(cubePositions[i]), mat4_rotation(to_radians(20.0f * i), (vec3){1.0f, 0.3f, 0.5f}));
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

	const GLfloat cameraSpeed = 0.05f;

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

static gboolean scroll_event(GtkGLArea *drawing, GdkEvent *event, gpointer user_data)
{
	if (gdk_event_get_event_type(event) != GDK_SCROLL) {
		return FALSE;
	}

	gdouble xoffset, yoffset;

	if (gdk_event_get_scroll_deltas(event, &xoffset, &yoffset) == FALSE) {
		GdkScrollDirection direction;

		if (gdk_event_get_scroll_direction(event, &direction) == FALSE) {
			return FALSE;
		}
		xoffset = direction == GDK_SCROLL_LEFT ? -1 : (direction == GDK_SCROLL_RIGHT ? 1 : 0);
		yoffset = direction == GDK_SCROLL_UP ? -1 : (direction == GDK_SCROLL_DOWN ? 1 : 0);
	};

	fov -= yoffset;
	if (fov <= FOV_MIN) {
		fov = FOV_MIN;
	}
	else if (fov >= FOV_MAX) {
		fov = FOV_MAX;
	}
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
	g_signal_connect(G_OBJECT(drawing), "scroll-event", G_CALLBACK(scroll_event), NULL);
	g_signal_connect(G_OBJECT(drawing), "key-press-event", G_CALLBACK(key_press_event), NULL);
	gtk_widget_add_events(drawing, GDK_ENTER_NOTIFY_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_SCROLL_MASK);
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
