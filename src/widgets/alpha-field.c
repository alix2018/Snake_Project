
#include "alpha-field.h"

struct _AlphaField
{
    ClutterActor parent_instance;

    ClutterActor *text;
};

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

    layout = clutter_bin_layout_new(CLUTTER_BIN_ALIGNMENT_FILL,
                                    CLUTTER_BIN_ALIGNMENT_CENTER);

    self->text = clutter_text_new_with_text("Sans 12", "");
    g_object_set(
        self->text,
        "editable", TRUE,
        "selectable", TRUE,
        "reactive", TRUE,
        "cursor-visible", TRUE,
        "x-expand", TRUE,
        NULL
    );
    clutter_text_set_color(CLUTTER_TEXT(self->text), CLUTTER_COLOR_Red);
    clutter_actor_set_margin(self->text, &margin);

    clutter_actor_set_layout_manager(CLUTTER_ACTOR(self), layout);
    clutter_actor_add_child(CLUTTER_ACTOR(self), self->text);
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
