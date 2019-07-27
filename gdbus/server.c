
#include <glib.h>
#include <gio/gio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "comm.h"
#include "test.h"

Interface1* itf1=NULL;
Interface2* itf2=NULL;

static void handle_print(
        GObject* gobj,
        GDBusMethodInvocation *invocation,
        const gchar *value,
        private_data *pdat)
{
    g_print("handle_print: %s\n", value);
}

static void handle_add(
    GObject* gobj,
    GDBusMethodInvocation *invocation,
    gint va,
    gint vb,
    private_data *pdat)
{
    g_print("handle_add: %d + %d\n", va, vb);
    //
    gint sum = va + vb;
    gchar *sum_str = calloc(64, 1);//这里必须使用内存分配 释放由 interface1_complete_add
    //
    snprintf(sum_str, 64, "%d", sum);
    //
    interface1_complete_add(gobj, invocation, sum, sum_str);
}

static void handle_transfer(
    GObject* gobj,
    GDBusMethodInvocation *invocation,
    GVariant *in_data,
    private_data *pdat)
{
    test_strct val;
    de_pkg(in_data, &val, sizeof(val));
    //
    g_print("handle_transfer: i=%d, s=%s, d=%lf, f=%f/%f/%f\n", 
        val.i, val.s, val.d, val.f[0], val.f[1], val.f[2]);
    //
    val.i = 10010;
    val.s[1] = '&';
    val.d = 1.23456;
    val.f[0] = 1.111;
    val.f[1] = 2.222;
    val.f[2] = 3.333;
    //
    interface2_complete_transfer(gobj, invocation, en_pkg(&val, sizeof(val)));
}

static gboolean emit_signal_boradcast(private_data *pdat)          
{
    gchar *signal_data = "123abc";
    interface1_emit_boradcast(itf1, signal_data);
    return TRUE;                                          
}

void bus_acquired_handler(
        GDBusConnection *connection,
        const gchar *name,
        private_data *pdat)
{
    GError *error = NULL;
    
    pdat->isRun = 1;

    printf("bus_acquired_handler has been invoked\n");
    printf("bus_acquired_handler the name = %s\n",name);
    //
    itf1 = interface1_skeleton_new();
    g_signal_connect(itf1, "handle-print", G_CALLBACK(handle_print), pdat);
    g_signal_connect(itf1, "handle-add", G_CALLBACK(handle_add), pdat);
    //
    g_dbus_interface_skeleton_export(
            G_DBUS_INTERFACE_SKELETON(itf1), 
            connection, 
            "/org/wx/test/interface1", 
            &error);
    if(error != NULL){                                                           
        g_print("Error: itf1 Failed to export object. Reason: %s.\n", error->message);
        g_error_free(error);                                                     
    }
    //
    itf2 = interface2_skeleton_new();
    g_signal_connect(itf2, "handle-transfer", G_CALLBACK(handle_transfer), pdat);
    //
    g_dbus_interface_skeleton_export(
            G_DBUS_INTERFACE_SKELETON(itf2), 
            connection, 
            "/org/wx/test/interface2", 
            &error);
    if(error != NULL){                                                           
        g_print("Error: itf2 Failed to export object. Reason: %s.\n", error->message);
        g_error_free(error);                                                     
    }                                                                    
}

void name_acquired_handler(
        GDBusConnection *connection,
        const gchar *name,
        private_data *pdat)
{
    printf("name_acquired_handler has been invoked\n");
}

void name_lost_handler(
        GDBusConnection *connection,
        const gchar *name,
        private_data *pdat)
{
    printf("GBusNameLost_Callback has been invoked\n");
}

int main(void)
{
    private_data pdat;
    GMainLoop* loop = NULL;
    //
    guint own_id = g_bus_own_name (
            G_BUS_TYPE_SESSION,
            "org.wx.test",
            G_BUS_NAME_OWNER_FLAGS_NONE,
            bus_acquired_handler,
            name_acquired_handler,
            name_lost_handler,
            (gpointer)&pdat, // user_data
            NULL);           // user_data_free_func()
    //
    g_timeout_add(1000, (GSourceFunc)emit_signal_boradcast, (gpointer)&pdat);
    //
    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    //
    g_bus_unown_name(own_id);
    return 0;
}
