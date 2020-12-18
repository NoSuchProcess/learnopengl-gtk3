#include <glib.h>
#include <gtk/gtk.h>

static void activate(GtkApplication *application, gpointer user_data)
{
	GtkWidget *window;

	window = gtk_application_window_new(application);

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
