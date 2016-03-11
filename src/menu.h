#include <clutter/clutter.h>
#include <stdlib.h>


gboolean jouer_clicked_cb(ClutterClickAction *action, ClutterActor *actor, gpointer data);
gboolean quitter_clicked_cb(ClutterClickAction *action, ClutterActor *actor, gpointer data);
gboolean button_enter_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
gboolean button_leave_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
