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

static void alpha_button_init_class(AlphaButtonClass *klass)
{
    G_OBJECT_CLASS(klass)->dispose = alpha_button_dispose;
    G_OBJECT_CLASS(klass)->finalize = alpha_button_finalize;
}

AlphaButton *alpha_button_new(GString *text)
{
    return NULL;
}
