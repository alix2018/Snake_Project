#include <clutter/clutter.h>

#ifndef ALPHA_SNAKE_ALPHA_BUTTON_H
#define ALPHA_SNAKE_ALPHA_BUTTON_H

G_BEGIN_DECLS

#define ALPHA_TYPE_BUTTON alpha_button_get_type()
G_DECLARE_FINAL_TYPE(AlphaButton, alpha_button, ALPHA, BUTTON, ClutterActor)

AlphaButton *alpha_button_new(GString *text);

G_END_DECLS

#endif //ALPHA_SNAKE_ALPHA_BUTTON_H
