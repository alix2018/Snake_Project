//
// Created by thibaut on 14/03/16.
//

#include "alpha-button.h"

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
    ClutterColor *bg = clutter_color_new(255, 255, 255, 255);

    clutter_actor_set_background_color(actor, bg);

    clutter_color_free(bg);

    return CLUTTER_EVENT_STOP;
}

static gboolean leave_event_cb(ClutterActor *actor, ClutterEvent *event,
                               gpointer data)
{
    ClutterColor *bg = clutter_color_alloc();
    clutter_color_from_string(bg, "grey");

    clutter_actor_set_background_color(actor, bg);

    clutter_color_free(bg);

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
    
    G_OBJECT_CLASS(alpha_button_parent_class)->dispose(object);
}

static void alpha_button_finalize(GObject *g_object)
{

    G_OBJECT_CLASS(alpha_button_parent_class)->finalize(g_object);
}

static void alpha_button_init(AlphaButton *self)
{
    ClutterMargin margin = (ClutterMargin) { 5, 5, 5, 5 };
    ClutterColor *bg = clutter_color_alloc();
    ClutterColor *fg = clutter_color_alloc();
    ClutterLayoutManager *layout = clutter_bin_layout_new(CLUTTER_BIN_ALIGNMENT_CENTER,
                                                          CLUTTER_BIN_ALIGNMENT_CENTER);

    clutter_color_from_string(bg, "grey");
    clutter_color_from_string(fg, "black");

    g_object_set(
        self,
        "background-color", bg,
        "reactive", TRUE,
        "layout-manager", layout,
        NULL
    );
    
    self->text = clutter_text_new();
    g_object_set(
        self->text,
        "color", fg,
        NULL
    );
    clutter_actor_set_margin(self->text, &margin);
    
    clutter_actor_add_child(CLUTTER_ACTOR(self), self->text);

    g_signal_connect(self, "enter-event", G_CALLBACK(enter_event_cb), NULL);
    g_signal_connect(self, "leave-event", G_CALLBACK(leave_event_cb), NULL);
}

static void alpha_button_class_init(AlphaButtonClass *klass)
{
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
