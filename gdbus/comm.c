
#include "comm.h"
#include "string.h"

void var_enpkg(GVariant **var, void *data, int dataLen)
{
    GVariantBuilder *builder;
    GVariant *tar;
    gchar *p = data;
    int i;

    builder = g_variant_builder_new (G_VARIANT_TYPE ("ay"));
    for(i = 0; i < dataLen; i++)
        g_variant_builder_add (builder, "y", *p++);
    tar = g_variant_builder_end (builder);
    *var = g_variant_new_variant(tar);
    // g_variant_unref(tar);
}

void def_depkg(GVariant *var, void *data, int dataLen)
{
    GVariant *tar, *tv;
    g_variant_get(var, "v", &tv);
    g_variant_get(tv, "ay", &tar);
    gchar *p;
    p =  data;
    //
    int i = 1;
    while(g_variant_iter_loop(tar, "y", p++))
    {
        if(++i > dataLen)
            break;
    }
    // g_variant_unref(tar);
    // g_variant_unref(tv);
}
