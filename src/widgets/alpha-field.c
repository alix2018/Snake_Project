
#include "alpha-field.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "utils.h"

G_DEFINE_TYPE(AlphaField, alpha_field, CLUTTER_TYPE_ACTOR)

enum
{
    PROP_0,
    PROP_FIELD_TEXT,
    PROP_N
};

static GParamSpec *obj_properties[PROP_N] = { NULL, };

static void alpha_field_set_property(GObject *object,
                                     guint property_id,
                                     const GValue *value,
                                     GParamSpec *pspec)
{
    AlphaField *self = ALPHA_FIELD(object);

    switch (property_id)
    {
        case PROP_FIELD_TEXT:
            clutter_text_set_text(CLUTTER_TEXT(self->text), g_value_get_string(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void alpha_field_get_property(GObject *object,
                                     guint property_id,
                                     GValue *value,
                                     GParamSpec *pspec)
{
    AlphaField *self = ALPHA_FIELD(object);

    switch (property_id)
    {
        case PROP_FIELD_TEXT:
            g_value_set_string(value, clutter_text_get_text(CLUTTER_TEXT(self->text)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void alpha_field_dispose(GObject *object)
{
    AlphaField *self = ALPHA_FIELD(object);

    g_clear_object(&self->text);
    
    g_clear_object(&self->img_b);
    g_clear_object(&self->img_bd);
    g_clear_object(&self->img_bg);
    g_clear_object(&self->img_d);
    g_clear_object(&self->img_hd);
    
    G_OBJECT_CLASS(alpha_field_parent_class)->dispose(object);
}

static void alpha_field_finalize(GObject *object)
{


    G_OBJECT_CLASS(alpha_field_parent_class)->finalize(object);
}

static void alpha_field_init(AlphaField *self)
{
    ClutterMargin margin = (ClutterMargin) { 5, 5, 5, 5 };
    ClutterLayoutManager *layout;
    ClutterActor *part;
    ClutterColor *bg = clutter_color_new(249, 249, 249, 255);
    
    self->img_b = create_clutter_image("data/bouton_clair_b.png");
    self->img_bd = create_clutter_image("data/bouton_clair_bd.png");
    self->img_bg = create_clutter_image("data/bouton_clair_bg.png");
    self->img_d = create_clutter_image("data/bouton_clair_d.png");
    self->img_hd = create_clutter_image("data/bouton_clair_hd.png");

    layout = clutter_grid_layout_new();
    clutter_actor_set_layout_manager(CLUTTER_ACTOR(self), layout);
    
    // Bas
    part = clutter_actor_new();
    clutter_actor_set_height(part, 3);
    clutter_actor_set_x_expand(part, TRUE);
    clutter_actor_set_content(part, CLUTTER_CONTENT(self->img_b));
    clutter_actor_set_content_repeat(part, CLUTTER_REPEAT_X_AXIS);
    clutter_grid_layout_attach(CLUTTER_GRID_LAYOUT(layout), part, 1, 2, 1, 1);
    
    // Bas Droite
    part = clutter_actor_new();
    clutter_actor_set_size(part, 4, 3);
    clutter_actor_set_content(part, CLUTTER_CONTENT(self->img_bd));
    clutter_grid_layout_attach(CLUTTER_GRID_LAYOUT(layout), part, 2, 2, 1, 1);
    
    // Bas Gauche
    part = clutter_actor_new();
    clutter_actor_set_size(part, 4, 3);
    clutter_actor_set_content(part, CLUTTER_CONTENT(self->img_bg));
    clutter_grid_layout_attach(CLUTTER_GRID_LAYOUT(layout), part, 0, 2, 1, 1);
    
    // Droite
    part = clutter_actor_new();
    clutter_actor_set_width(part, 4);
    clutter_actor_set_y_expand(part, TRUE);
    clutter_actor_set_content(part, CLUTTER_CONTENT(self->img_d));
    clutter_actor_set_content_repeat(part, CLUTTER_REPEAT_Y_AXIS);
    clutter_grid_layout_attach(CLUTTER_GRID_LAYOUT(layout), part, 2, 1, 1, 1);
    
    // Haut Droite
    part = clutter_actor_new();
    clutter_actor_set_size(part, 4, 3);
    clutter_actor_set_content(part, CLUTTER_CONTENT(self->img_hd));
    clutter_grid_layout_attach(CLUTTER_GRID_LAYOUT(layout), part, 2, 0, 1, 1);
    
    // Milieu
    part = clutter_actor_new();
    clutter_actor_set_layout_manager(part, clutter_bin_layout_new(CLUTTER_BIN_ALIGNMENT_CENTER, CLUTTER_BIN_ALIGNMENT_CENTER));
    clutter_actor_set_x_expand(part, TRUE);
    clutter_actor_set_y_expand(part, TRUE);
    clutter_actor_set_background_color(part, bg);
    clutter_grid_layout_attach(CLUTTER_GRID_LAYOUT(layout), part, 0, 0, 2, 2);

    self->text = clutter_text_new_with_text("Sans 12", "");
    g_object_set(
        self->text,
        "editable", TRUE,
        "selectable", TRUE,
        "reactive", TRUE,
        "cursor-visible", TRUE,
        "x-expand", TRUE,
        "color", CLUTTER_COLOR_Black,
        "single-line-mode", TRUE,
        NULL
    );
    clutter_actor_set_margin(self->text, &margin);

    clutter_actor_add_child(part, self->text);
}

static void alpha_field_class_init(AlphaFieldClass *klass)
{
    G_OBJECT_CLASS(klass)->get_property = alpha_field_get_property;
    G_OBJECT_CLASS(klass)->set_property = alpha_field_set_property;

    obj_properties[PROP_FIELD_TEXT] = g_param_spec_string(
        "text",
        "Texte du champ",
        "Permet de récupérer ou de modifier le texte du champ",
        "",
        G_PARAM_READWRITE
    );

    g_object_class_install_properties(G_OBJECT_CLASS(klass), PROP_N, obj_properties);

    G_OBJECT_CLASS(klass)->dispose = alpha_field_dispose;
    G_OBJECT_CLASS(klass)->finalize = alpha_field_finalize;
}
