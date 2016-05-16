#include "alpha-check-box.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "utils.h"

G_DEFINE_TYPE(AlphaCheckBox, alpha_check_box, CLUTTER_TYPE_ACTOR)

enum
{
    PROP_0,
    PROP_CHECK_BOX_COLOR_STRING,
    PROP_CHECK_BOX_COLOR,
    PROP_CHECK_BOX_CHECKED,
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

static gboolean clicked_cb(ClutterClickAction *action, ClutterActor *actor, gpointer data)
{
    gboolean checked;
    
    g_object_get(actor, "checked", &checked, NULL);
    g_object_set(actor, "checked", !checked, NULL);
    
    return CLUTTER_EVENT_STOP;
}

static void alpha_check_box_set_property(GObject *object,
                                      guint property_id,
                                      const GValue *value,
                                      GParamSpec *pspec)
{
    AlphaCheckBox *self = ALPHA_CHECK_BOX(object);

    switch (property_id)
    {
        case PROP_CHECK_BOX_CHECKED:
            g_value_get_boolean(value) ? clutter_actor_show(self->coche) : clutter_actor_hide(self->coche);
            break;
         
        case PROP_CHECK_BOX_COLOR:
            break;
            
        case PROP_CHECK_BOX_COLOR_STRING:
            clutter_color_from_string(self->color, g_value_get_string(value));
            ClutterEffect *effect = clutter_colorize_effect_new(self->color);
            clutter_actor_add_effect(CLUTTER_ACTOR(self->fond), effect);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void alpha_check_box_get_property(GObject *object,
                                      guint property_id,
                                      GValue *value,
                                      GParamSpec *pspec)
{
    AlphaCheckBox *self = ALPHA_CHECK_BOX(object);

    switch (property_id)
    {
        case PROP_CHECK_BOX_COLOR:
            clutter_value_set_color(value, self->color);
            break;
            
        case PROP_CHECK_BOX_CHECKED:
            g_value_set_boolean(value, CLUTTER_ACTOR_IS_VISIBLE(self->coche));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

static void alpha_check_box_dispose(GObject *object)
{
    AlphaCheckBox *self = ALPHA_CHECK_BOX(object);
    
    g_clear_object(&self->img_b);
    g_clear_object(&self->img_bd);
    g_clear_object(&self->img_bg);
    g_clear_object(&self->img_d);
    g_clear_object(&self->img_hd);
    
    g_clear_object(&self->coche);
    g_clear_object(&self->fond);
    
    G_OBJECT_CLASS(alpha_check_box_parent_class)->dispose(object);
}

static void alpha_check_box_finalize(GObject *g_object)
{
    AlphaCheckBox *self = ALPHA_CHECK_BOX(g_object);
    
    clutter_color_free(self->color);
    
    G_OBJECT_CLASS(alpha_check_box_parent_class)->finalize(g_object);
}

static void alpha_check_box_init(AlphaCheckBox *self)
{
    ClutterMargin margin = (ClutterMargin) { 5, 9, 5, 8 };
    ClutterColor *fg = clutter_color_alloc();
    ClutterColor *bg = clutter_color_new(249, 249, 249, 255);
    ClutterLayoutManager *layout = clutter_grid_layout_new();
    ClutterActor *part;
    ClutterAction *action = clutter_click_action_new();
    
    self->img_b = create_clutter_image("data/bouton_clair_b.png");
    self->img_bd = create_clutter_image("data/bouton_clair_bd.png");
    self->img_bg = create_clutter_image("data/bouton_clair_bg.png");
    self->img_d = create_clutter_image("data/bouton_clair_d.png");
    self->img_hd = create_clutter_image("data/bouton_clair_hd.png");
    
    self->color = clutter_color_alloc();

    clutter_color_from_string(fg, "white");

    g_object_set(
        self,
        "reactive", TRUE,
        "layout-manager", clutter_bin_layout_new(CLUTTER_BIN_ALIGNMENT_CENTER, CLUTTER_BIN_ALIGNMENT_CENTER),
        NULL
    );
    clutter_actor_set_pivot_point(CLUTTER_ACTOR(self), 0.5, 0.5);
    
    self->fond = clutter_actor_new();
    clutter_actor_add_child(CLUTTER_ACTOR(self), self->fond);
    clutter_actor_set_size(self->fond, 32, 32);
    clutter_actor_set_layout_manager(self->fond, layout);
    
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
    
    self->coche = clutter_actor_new();
    clutter_actor_set_size(self->coche, 18, 18);
    clutter_actor_set_background_color(self->coche, fg);
    clutter_actor_add_child(CLUTTER_ACTOR(self), self->coche);
    clutter_actor_hide(self->coche);
    
    clutter_actor_add_action(CLUTTER_ACTOR(self), action);
    g_signal_connect(action, "clicked", G_CALLBACK(clicked_cb), self);
    
    clutter_color_free(bg);
    clutter_color_free(fg);

    g_signal_connect(self, "enter-event", G_CALLBACK(enter_event_cb), NULL);
    g_signal_connect(self, "leave-event", G_CALLBACK(leave_event_cb), NULL);
}

static void alpha_check_box_class_init(AlphaCheckBoxClass *klass)
{
    GdkPixbuf *pixbuf;
    
    G_OBJECT_CLASS(klass)->set_property = alpha_check_box_set_property;
    G_OBJECT_CLASS(klass)->get_property = alpha_check_box_get_property;
    
    obj_properties[PROP_CHECK_BOX_COLOR_STRING] = g_param_spec_string(
        "color-string",
        "",
        "",
        "#00000000",
        G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE
    );

    obj_properties[PROP_CHECK_BOX_COLOR] = clutter_param_spec_color(
        "color",
        "La couleur de la case à cocher",
        "Modifie la couleur de la case à cocher",
        CLUTTER_COLOR_Transparent,
        G_PARAM_READABLE
    );
    
    obj_properties[PROP_CHECK_BOX_CHECKED] = g_param_spec_boolean(
        "checked",
        "Case cochée",
        "Case cochée",
        FALSE,
        G_PARAM_READWRITE
    );

    g_object_class_install_properties(G_OBJECT_CLASS(klass),
                                      PROP_N,
                                      obj_properties);

    G_OBJECT_CLASS(klass)->dispose = alpha_check_box_dispose;
    G_OBJECT_CLASS(klass)->finalize = alpha_check_box_finalize;
}
