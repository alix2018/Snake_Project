/**
 * @file      alpha-button.h
 * @author    alpha-snake
 * @version   1
 * @date      16/05/2016
 * @brief     Classe AlphaButton pour le menu.
 * @details   ---
 */

#include <clutter/clutter.h>

#ifndef ALPHA_SNAKE_ALPHA_BUTTON_H
#define ALPHA_SNAKE_ALPHA_BUTTON_H


#define ALPHA_TYPE_BUTTON (alpha_button_get_type())
#define ALPHA_BUTTON(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), ALPHA_TYPE_BUTTON, AlphaButton))
#define ALPHA_IS_BUTTON(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), ALPHA_TYPE_BUTTON))
#define ALPHA_BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), ALPHA_TYPE_BUTTON,AlphaButtonClass))
#define ALPHA_IS_BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), ALPHA_TYPE_BUTTON))
#define ALPHA_BUTTON_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), ALPHA_TYPE_BUTTON, AlphaButtonClass))


typedef struct _AlphaButton AlphaButton;
typedef struct _AlphaButtonClass AlphaButtonClass;

struct _AlphaButton
{
    ClutterActor parent_instance;

    ClutterActor *text;
    
    ClutterImage *img_hd;
    ClutterImage *img_d;
    ClutterImage *img_bd;
    ClutterImage *img_b;
    ClutterImage *img_bg;
};

struct _AlphaButtonClass
{
    ClutterActorClass parent_class;
};

GType alpha_button_get_type();

AlphaButton *alpha_button_new(gchar *text);

#endif //ALPHA_SNAKE_ALPHA_BUTTON_H
