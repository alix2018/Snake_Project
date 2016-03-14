//
// Created by thibaut on 14/03/16.
//

#include "alpha-button.h"

struct _AlphaButton
{
    ClutterActor parent_instance;

    // Les attributs ici
};

G_DEFINE_TYPE(AlphaButton, alpha_button, CLUTTER_TYPE_ACTOR)

enum
{
    PROP_0,
    PROP_BUTTON_TEXT,
    PROP_N
};

static GParamSpec *obj_properties[PROP_N] = { NULL, };

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
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void alpha_button_dispose(GObject *self)
{
    /*
     * Destructeur pour les objets de clutter
     *
     * Il ne faut pas utiliser free pour les détruire, mais
     * g_object_clear
     */

    G_OBJECT_CLASS(alpha_button_parent_class)->dispose(self);
}

static void alpha_button_finalize(GObject *g_object)
{
    /*
     * Destructeur pour les objets non clutter.
     */

    G_OBJECT_CLASS(alpha_button_parent_class)->finalize(g_object);
}

static void alpha_button_init(AlphaButton *self)
{
    // Le constructeur (enfin je crois ...)
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
