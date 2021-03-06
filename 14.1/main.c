#include <stddef.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <epoxy/gl.h>
#include <shader_make.h>
#include <glmath.h>

typedef struct {
	vec3 position;
	vec3 normal;
} vertex;

static vertex vertices[] = {
	{{+0.5f, +0.5f, +0.5f}, {+1.0f, 0.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {+1.0f, 0.0f, 0.0f}},
	{{+0.5f, +0.5f, -0.5f}, {+1.0f, 0.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {+1.0f, 0.0f, 0.0f}},
	{{+0.5f, -0.5f, -0.5f}, {+1.0f, 0.0f, 0.0f}},
	{{+0.5f, +0.5f, -0.5f}, {+1.0f, 0.0f, 0.0f}},

	{{+0.5f, +0.5f, +0.5f}, {0.0f, +1.0f, 0.0f}},
	{{+0.5f, +0.5f, -0.5f}, {0.0f, +1.0f, 0.0f}},
	{{-0.5f, +0.5f, -0.5f}, {0.0f, +1.0f, 0.0f}},
	{{+0.5f, +0.5f, +0.5f}, {0.0f, +1.0f, 0.0f}},
	{{-0.5f, +0.5f, -0.5f}, {0.0f, +1.0f, 0.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, +1.0f, 0.0f}},

	{{-0.5f, -0.5f, +0.5f}, {-1.0f, 0.0f, 0.0f}},
	{{-0.5f, +0.5f, +0.5f}, {-1.0f, 0.0f, 0.0f}},
	{{-0.5f, +0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
	{{-0.5f, -0.5f, +0.5f}, {-1.0f, 0.0f, 0.0f}},
	{{-0.5f, +0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},

	{{+0.5f, -0.5f, +0.5f}, {0.0f, -1.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
	{{+0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
	{{+0.5f, -0.5f, +0.5f}, {0.0f, -1.0f, 0.0f}},
	{{-0.5f, -0.5f, +0.5f}, {0.0f, -1.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},

	{{+0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}},
	{{+0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}},
	{{+0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}},
	{{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}},
	{{-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}},

	{{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
	{{+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
	{{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
	{{-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}}
};

static GLuint vao;
static GLuint vbo;
static GLuint program;

static GLuint light_vao;
static GLuint light_vbo;
static GLuint light_program;

static vec3 cameraPos = { 0.0f, 0.0f, 5.0f };
static vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
static vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

#define FOV_MAX 60.
#define FOV_MIN 1.

static gdouble fov = 45., yaw = -90., pitch = 0;
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

	glClearColor(0.2, 0.3, 0.3, 1.0);

	light_program = shader_make(SHADER_SET_LIGHT);

	{
		GLint index;

		glGenVertexArrays(1, &light_vao);
		glBindVertexArray(light_vao);

		glGenBuffers(1, &light_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

		index = glGetAttribLocation(light_program, "position");
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, position));
		glEnableVertexAttribArray(index);

		glBindVertexArray(0);
	}

	program = shader_make(SHADER_SET_CONTAINER);

	{
		GLint index;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

		index = glGetAttribLocation(program, "position");
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, position));
		glEnableVertexAttribArray(index);
		index = glGetAttribLocation(program, "aNormal");
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof (vertex), (const GLvoid *) offsetof(vertex, normal));
		glEnableVertexAttribArray(index);

		glBindVertexArray(0);
	}

	timer = g_timer_new();
}

static void unrealize(GtkGLArea *area, gpointer user_data)
{
	g_timer_destroy(timer);

	gtk_gl_area_make_current(area);
	if (gtk_gl_area_get_error(area) != NULL) {
		return;
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);

	glDeleteVertexArrays(1, &light_vao);
	glDeleteBuffers(1, &light_vbo);
	glDeleteProgram(light_program);
}

static gboolean render(GtkGLArea *area, GdkGLContext *context, gpointer user_data)
{
	mat4 model;
	const mat4 view = mat4_look_at(cameraPos, vec3_add(cameraPos, cameraFront), cameraUp);
	const vec3 lightPos = mat4_mulv3(mat4_rotation_x(g_timer_elapsed(timer, NULL)), (vec3) { 1.2f, 1.0f, 2.0f });

	const GLint width = gtk_widget_get_allocated_width(GTK_WIDGET(area));
	const GLint height = gtk_widget_get_allocated_height(GTK_WIDGET(area));
	const mat4 projection = mat4_perspective(radians(fov), ((GLfloat) width) / ((GLfloat) height), 1., 100.);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// lamp
	model = mat4_transformation((vec3) { 0.2f, 0.2f, 0.2f }, lightPos);

	glUseProgram(light_program);
	glUniformMatrix4fv(glGetUniformLocation(light_program, "model"), 1, GL_FALSE, (const GLfloat *) &model);
	glUniformMatrix4fv(glGetUniformLocation(light_program, "view"), 1, GL_FALSE, (const GLfloat *) &view);
	glUniformMatrix4fv(glGetUniformLocation(light_program, "projection"), 1, GL_FALSE, (const GLfloat *) &projection);
	glBindVertexArray(light_vao);
	glDrawArrays(GL_TRIANGLES, 0, G_N_ELEMENTS(vertices));

	// Container
	model = mat4_identity();

	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "lightColor"), 1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, (const GLfloat *) &lightPos);
	glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, (const GLfloat *) &cameraPos);

	glUniform3f(glGetUniformLocation(program, "material.ambient"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(program, "material.specular"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(program, "material.shininess"), 32.0f);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const GLfloat *) &model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (const GLfloat *) &view);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (const GLfloat *) &projection);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, G_N_ELEMENTS(vertices));

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
		.x = cos(radians(pitch)) * cos(radians(yaw)),
		.y = sin(radians(pitch)),
		.z = cos(radians(pitch)) * sin(radians(yaw))
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
	}

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

static gboolean ontick(GtkWidget *drawing, GdkFrameClock *frame_clock, gpointer user_data)
{
	GLfloat currentFrame = g_timer_elapsed(timer, NULL);

	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	gtk_gl_area_queue_render(GTK_GL_AREA(drawing));

	return G_SOURCE_CONTINUE;
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
	gtk_widget_add_tick_callback(drawing, ontick, NULL, NULL);
	gtk_widget_add_events(drawing, GDK_ENTER_NOTIFY_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK);
	gtk_widget_set_can_focus(drawing, TRUE);

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
