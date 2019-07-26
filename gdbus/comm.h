#ifndef __COMM_H__
#define __COMM_H__

#include <glib.h>
#include <gio/gio.h>

void var_enpkg(GVariant **var, void *data, int dataLen);
void def_depkg(GVariant *var, void *data, int dataLen);

#endif