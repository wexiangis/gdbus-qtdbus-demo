
#include "comm.h"
#include "string.h"

GVariant *en_pkg(void *data, int dataLen)
{
    GVariantBuilder *builder;
    gchar *p = data;
    if(dataLen < 1)
        return NULL;
    builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
    do{
        g_variant_builder_add(builder, "y", *p++);
    }while(--dataLen);
    return g_variant_new_variant(g_variant_builder_end(builder));
}

void de_pkg(GVariant *var, void *data, int dataLen)
{
    GVariant *tar, *tv;
    gchar *p = data;
    if(dataLen < 1)
        return;
    g_variant_get(var, "v", &tv);
    g_variant_get(tv, "ay", &tar);
    while(g_variant_iter_loop(tar, "y", p++) && --dataLen);
    // g_variant_unref(tar);
    // g_variant_unref(tv);
}
