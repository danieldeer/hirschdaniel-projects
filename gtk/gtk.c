#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

GtkApplication *app;

static void spawn_window(GtkApplication *app, gpointer user_data);

static void print_hello (GtkWidget *widget,
             gpointer   data)
{
    GMutex mutex;
    gint64 end_time;
    GCond cond;

        printf("time: %d\n", end_time);

        for(int i=0; i<100; i++)
        {
        spawn_window(app, data);
        }
}

static void spawn_window (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *button_box;

  window = gtk_application_window_new (app);
  gtk_window_set_deletable(GTK_WINDOW(window), FALSE);
  gtk_window_set_title (GTK_WINDOW (window), "Daniel's GTK App");

  int width = ((double) rand())/RAND_MAX * 800 + 100;
  int height = ((double) rand())/RAND_MAX * 500 + 100;
  gtk_window_set_default_size (GTK_WINDOW (window), width, height);

  GdkColor color;
    color.green = rand();
    color.blue = rand();
    color.red = rand();
  gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add (GTK_CONTAINER (window), button_box);

  button = gtk_button_new_with_label ("Click Me!!!");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  gtk_container_add (GTK_CONTAINER (button_box), button);

  gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
  srand(time(NULL));
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (spawn_window), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

