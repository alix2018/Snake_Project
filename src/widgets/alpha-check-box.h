#include <clutter/clutter.h>

#ifndef ALPHA_SNAKE_ALPHA_BUTTON_H
#define ALPHA_SNAKE_ALPHA_BUTTON_H


#define ALPHA_TYPE_CHECK_BOX (alpha_check_box_get_type())
#define ALPHA_CHECK_BOX(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), ALPHA_TYPE_CHECK_BOX, AlphaCheckBox))
#define ALPHA_IS_CHECK_BOX(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), ALPHA_TYPE_CHECK_BOX))
#define ALPHA_CHECK_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), ALPHA_TYPE_CHECK_BOX,AlphaCheckBoxClass))
#define ALPHA_IS_CHECK_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), ALPHA_TYPE_CHECK_BOX))
#define ALPHA_CHECK_BOX_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), ALPHA_TYPE_CHECK_BOX, AlphaCheckBoxClass))


typedef struct _AlphaCheckBox AlphaCheckBox;
typedef struct _AlphaCheckBoxClass AlphaCheckBoxClass;

struct _AlphaCheckBox
{
    ClutterActor parent_instance;

    ClutterActor *coche;
    ClutterActor *fond;
    ClutterColor *color;
    
    ClutterImage *img_hd;
    ClutterImage *img_d;
    ClutterImage *img_bd;
    ClutterImage *img_b;
    ClutterImage *img_bg;
};

struct _AlphaCheckBoxClass
{
    ClutterActorClass parent_class;
};

GType alpha_check_box_get_type();

#endif //ALPHA_SNAKE_ALPHA_BUTTON_H
