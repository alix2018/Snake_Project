#include <gtk/gtk.h>
#include <clutter/clutter.h>
#include "partie.h"


static void
print_quitter (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Quitter\n");
}

static void
print_options (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Options\n");
}


static void
lancer_snake (GtkWidget *widget,
               gpointer   data)
{
  ClutterScript *ui;
  Partie *partie;


  ui = clutter_script_new();
  clutter_script_load_from_file(ui, "src/gui/stage.json", NULL);
  clutter_script_connect_signals(ui, ui);

  partie = create_partie();// partie.c
  init_partie(partie, ui, 30, 30);// partie.c
  clutter_main();

  free_partie(partie);
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
  
  {

  GtkWidget *window;
  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *button3;
  GtkWidget *box;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Menu");
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);

  box = gtk_box_new(TRUE, 0);
  gtk_container_add(GTK_CONTAINER(window), box);


  button1 = gtk_button_new_with_label ("Jouer");
  gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE, 0);
  g_signal_connect (GTK_BUTTON (button1), "clicked", G_CALLBACK (lancer_snake), G_OBJECT (window));


  button3 = gtk_button_new_with_label ("Options");
  gtk_box_pack_start(GTK_BOX(box), button3, FALSE, FALSE, 0);
  g_signal_connect (GTK_BUTTON (button3), "clicked", G_CALLBACK (print_options), G_OBJECT (window));


  button2 = gtk_button_new_with_label ("Quitter");
  gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 0);
  g_signal_connect (GTK_BUTTON (button2), "clicked", G_CALLBACK (print_quitter), G_OBJECT (window));
  g_signal_connect_swapped (button2, "clicked", G_CALLBACK (gtk_widget_destroy), window);


  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}