#include <clutter/clutter.h>

#ifndef ALPHA_SNAKE_ALPHA_BUTTON_H
#define ALPHA_SNAKE_ALPHA_BUTTON_H

G_BEGIN_DECLS

#define ALPHA_TYPE_BUTTON alpha_button_get_type()
//G_DECLARE_FINAL_TYPE(AlphaButton, alpha_button, ALPHA, BUTTON, ClutterActor)
#define ALPHA_BUTTON(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), ALPHA_TYPE_BUTTON, AlphaButton))
#define ALPHA_IS_BUTTON(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ALPHA_TYPE_BUTTON))
#define BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), ALPHA_TYPE_BUTTON,AlphaButtonClass))
#define ALPHA_IS_BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ALPHA_TYPE_BUTTON))
#define BUTTON_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), ALPHA_TYPE_BUTTON, AlphaButtonClass)


typedef struct _AlphaButton AlphaButton;
typedef struct _AlphaButtonClass AlphaButtonClass;

AlphaButton *alpha_button_new(gchar *text);

G_END_DECLS

#endif //ALPHA_SNAKE_ALPHA_BUTTON_H
