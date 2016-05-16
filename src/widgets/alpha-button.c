/**
 * @file      alpha-button.c
 * @author    alpha-snake
 * @version   1
 * @date      16/05/2016
 * @brief     Classe AlphaButton pour le menu.
 * @details   ---
 */
#include "alpha-button.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "utils.h"

G_DEFINE_TYPE(AlphaButton, alpha_button, CLUTTER_TYPE_ACTOR)

enum
{
    PROP_0,
    PROP_BUTTON_TEXT,
    PROP_N
};

static GParamSpec *obj_properties[PROP_N] = { NULL, };

static gboolean enter_event_cb(ClutterActor *actor, ClutterEvent *event,
                               gpointer data)
{
    clutter_actor_save_easing_state(actor);
    
    clutter_actor_set_easing_duration(actor, 200);
    clutter_actor_set_scale(actor, 1.05, 1.05);
    
    clutter_actor_set_z_position(actor, 6);
    
    clutter_actor_restore_easing_state(actor);

    return CLUTTER_EVENT_STOP;
}

static gboolean leave_event_cb(ClutterActor *actor, ClutterEvent *event,
                               gpointer data)
{
    clutter_actor_save_easing_state(actor);
    
    clutter_actor_set_easing_duration(actor, 200);
    clutter_actor_set_scale(actor, 1.0, 1.0);
    
    clutter_actor_set_z_position(actor, 0.0);
    
    clutter_actor_restore_easing_state(actor);

    return CLUTTER_EVENT_STOP;
}

static void alpha_button_set_property(GObject *object,
                                      guint property_id,
                                      const GValue *value,
                                      GParamSpec *pspec)
{
    AlphaButton *self = ALPHA_BUTTON(object);

    switch (property_id)
    {
        case PROP_BUTTON_TEXT:
            // changer le texte
            clutter_text_set_text(CLUTTER_TEXT(self->text), g_value_get_string(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void alpha_button_get_property(GObject *object,
                                      guint property_id,
                                      GValue *value,
                                      GParamSpec *pspec)
{
    AlphaButton *self = ALPHA_BUTTON(object);

    switch (property_id)
    {
        case PROP_BUTTON_TEXT:
            // Retourner le texte
            g_value_set_string(value,clutter_text_get_text(CLUTTER_TEXT(self->text)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void alpha_button_dispose(GObject *object)
{
    AlphaButton *self = ALPHA_BUTTON(object);

    g_clear_object(&self->text);
    
    g_clear_object(&self->img_b);
    g_clear_object(&self->img_bd);
    g_clear_object(&self->img_bg);
    g_clear_object(&self->img_d);
    g_clear_object(&self->img_hd);
    
    G_OBJECT_CLASS(alpha_button_parent_class)->dispose(object);
}

static void alpha_button_finalize(GObject *g_object)
{

    G_OBJECT_CLASS(alpha_button_parent_class)->finalize(g_object);
}

static void alpha_button_init(AlphaButton *self)
{
    ClutterMargin margin = (ClutterMargin) { 5, 5, 5, 5 };
    ClutterColor *fg = clutter_color_alloc();
    ClutterColor *bg = clutter_color_new(26, 26, 26, 255);
    ClutterLayoutManager *layout = clutter_grid_layout_new();
    ClutterActor *part;
    
    self->img_b = create_clutter_image("data/bouton_fonce_b.png");
    self->img_bd = create_clutter_image("data/bouton_fonce_bd.png");
    self->img_bg = create_clutter_image("data/bouton_fonce_bg.png");
    self->img_d = create_clutter_image("data/bouton_fonce_d.png");
    self->img_hd = create_clutter_image("data/bouton_fonce_hd.png");

    clutter_color_from_string(fg, "white");

    g_object_set(
        self,
        "reactive", TRUE,
        "layout-manager", layout,
        NULL
    );
    clutter_actor_set_pivot_point(CLUTTER_ACTOR(self), 0.5, 0.5);
    
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
    
    self->text = clutter_text_new();
    g_object_set(
        self->text,
        "color", fg,
        "font-name", "Sans bold",
        NULL
    );
    clutter_actor_set_margin(self->text, &margin);
    
    clutter_actor_add_child(part, self->text);
    
    clutter_color_free(bg);
    clutter_color_free(fg);

    g_signal_connect(self, "enter-event", G_CALLBACK(enter_event_cb), NULL);
    g_signal_connect(self, "leave-event", G_CALLBACK(leave_event_cb), NULL);
}

static void alpha_button_class_init(AlphaButtonClass *klass)
{
    GdkPixbuf *pixbuf;
    
    G_OBJECT_CLASS(klass)->set_property = alpha_button_set_property;
    G_OBJECT_CLASS(klass)->get_property = alpha_button_get_property;

    obj_properties[PROP_BUTTON_TEXT] = g_param_spec_string(
        "button-text",
        "Texte du bouton",
        "Modifie le texte du bouton",
        "Bouton",
        G_PARAM_READWRITE
    );

    g_object_class_install_properties(G_OBJECT_CLASS(klass),
                                      PROP_N,
                                      obj_properties);

    G_OBJECT_CLASS(klass)->dispose = alpha_button_dispose;
    G_OBJECT_CLASS(klass)->finalize = alpha_button_finalize;
}

AlphaButton *alpha_button_new(gchar *text)
{
    return ALPHA_BUTTON(g_object_new(ALPHA_TYPE_BUTTON, "button-text", text, NULL));
}
    
