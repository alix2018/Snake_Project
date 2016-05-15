
#include "utils.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

ClutterImage *create_clutter_image(const gchar *name)
{
    ClutterImage *image;
    GdkPixbuf *pixbuf;
    
    pixbuf = gdk_pixbuf_new_from_file(name, NULL);
    image = CLUTTER_IMAGE(clutter_image_new());
    clutter_image_set_data(
        image,
        gdk_pixbuf_get_pixels(pixbuf),
        gdk_pixbuf_get_has_alpha(pixbuf) ? COGL_PIXEL_FORMAT_RGBA_8888 : COGL_PIXEL_FORMAT_RGB_888,
        gdk_pixbuf_get_width(pixbuf),
        gdk_pixbuf_get_height(pixbuf),
        gdk_pixbuf_get_rowstride(pixbuf),
        NULL
    );
    g_object_unref(pixbuf);

    return image;
}