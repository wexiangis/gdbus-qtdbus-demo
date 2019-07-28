
#include <glib.h>
#include <gio/gio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "comm.h"
#include "test.h"

Interface1* itf1=NULL;
Interface2* itf2=NULL;

// 注意这里client端g_signal_connect()注册的回调
// 函数格式和server端的不一样, 
// 少了GDBusMethodInvocation *invocation参数
static gboolean handle_boradcast(
        GObject* gobj,
        const gchar *arg_data,
        gpointer user_data)
{
    g_print("handle_boradcast: %s\n", arg_data);
    return TRUE;
}

static void name_appeared_handler(
        GDBusConnection *connection,
        const gchar *name,
        const gchar *name_owner,
        private_data *pdat)
{
    GError *error = NULL;
    
    pdat->isRun = 1;
    
    g_print("name_appeared_handler: name %s, owner %s\n", name, name_owner);
    
    //
    itf1 = interface1_proxy_new_sync(
        connection,
        G_DBUS_PROXY_FLAGS_NONE, 
        "org.wx.test", 
        "/org/wx/test/interface1", 
        NULL, 
        &error);
    // itf1 = interface1_proxy_new_for_bus_sync (
    //     G_BUS_TYPE_SESSION,
    //     G_DBUS_PROXY_FLAGS_NONE,
    //     "org.wx.test", 
    //     "/org/wx/test/interface1", 
    //     NULL,
    //     &error);
    if(error != NULL){                                                           
        g_print("Error: itf1 Failed to proxy object. Reason: %s.\n", error->message);
        g_error_free(error);                                                     
    }
    
    pdat->handler_id = g_signal_connect(
        itf1, "boradcast", G_CALLBACK(handle_boradcast), pdat);
    
    //
    itf2 = interface2_proxy_new_sync(
        connection,
        G_DBUS_PROXY_FLAGS_NONE, 
        "org.wx.test", 
        "/org/wx/test/interface2", 
        NULL, 
        &error);
    // itf2 = interface2_proxy_new_for_bus_sync (
    //     G_BUS_TYPE_SESSION,
    //     G_DBUS_PROXY_FLAGS_NONE,
    //     "org.wx.test", 
    //     "/org/wx/test/interface2", 
    //     NULL,
    //     &error);
    if(error != NULL){                                                           
        g_print("Error: itf2 Failed to proxy object. Reason: %s.\n", error->message);
        g_error_free(error);                                                     
    }
}

static void name_vanished_handler
        (GDBusConnection *connection,
        const gchar *name,
        private_data *pdat)
{
    pdat->isRun = 0;
    //
    g_print("name_vanished_handler: name %s\n", name);
    //
    if(pdat->handler_id)
    {
        g_signal_handler_disconnect(itf1, pdat->handler_id);
        pdat->handler_id = 0;
    }
}

static void method_add_callback(
        GObject* gobj, 
        GAsyncResult* res, 
        private_data *pdat)
{
    GError *error = NULL;
    //
	gint sum = 0;
    gchar *sum_str = NULL;
    //
	interface1_call_add_finish(gobj, &sum, &sum_str, res, &error);
    g_print("method_add_callback: sum = %d, sum_str = \"%s\"\n", sum, sum_str);
}

static void method_transfer_callback(
        GObject* gobj, 
        GAsyncResult* res, 
        private_data *pdat)
{
    GError *error = NULL;
    //
    GVariant *out_data;
    //
	interface2_call_transfer_finish (
        gobj, &out_data,
        res, &error);
    //
    test_strct val;
    de_pkg(out_data, &val, sizeof(val));
    //
    g_print("method_transfer_callback: i=%d, s=%s, d=%lf, f=%f/%f/%f\n", 
        val.i, val.s, val.d, val.f[0], val.f[1], val.f[2]);
}

static gboolean timeout_handler(private_data *pdat)
{
    
    //-- 接口函数没有返回的 无须准备回调函数 --
    // if(IS_INTERFACE1_PROXY(itf1))
    if(pdat->isRun)
        interface1_call_print (
            itf1, "client-log-1234abcd",
            NULL, NULL, pdat
        );
    
    //-- 接口函数有返回的 需准备一个回调函数来处理返回的数据 --
    // if(IS_INTERFACE1_PROXY(itf1))
    if(pdat->isRun)
        interface1_call_add (
            itf1, 123, 432,
            NULL, method_add_callback, pdat
        );

    //-- 参数序列化 --
    test_strct val = {
        .i = 10086,
        .s = "abc",
        .d = 3.1415926,
        .f = {1.23, 4.56, 7.89},
    };
    // if(IS_INTERFACE2_PROXY(itf2))
    if(pdat->isRun)
        interface2_call_transfer(
            itf2, en_pkg(&val, sizeof(val)), 
            NULL, method_transfer_callback, pdat
        );
    //
    return TRUE;
}

int main (int argc, char *argv[])
{
    private_data pdat = {
        .isRun = 0,
        .handler_id = 0
    };
    GMainLoop *loop = NULL;
    guint watcher_id;
    //
    watcher_id = g_bus_watch_name (
            G_BUS_TYPE_SESSION,
            "org.wx.test",
            G_BUS_NAME_OWNER_FLAGS_NONE,
            name_appeared_handler,
            name_vanished_handler,
            (gpointer)&pdat, // user_data
            NULL);           // user_data_free_func()
    //
    g_timeout_add(1000, (GSourceFunc)timeout_handler, (gpointer)&pdat);
    //
    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
    //
    g_bus_unwatch_name (watcher_id);
    return 0;
}
