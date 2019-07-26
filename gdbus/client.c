
#include <glib.h>
#include <gio/gio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "comm.h"
#include "test.h"

static GDBusProxy *proxy_log = NULL;
static General_api *proxy_general = NULL;
 
void methodCallback (GObject* gobj, 
        GAsyncResult* res, 
        gpointer user_data)
{
	g_printf("methodCallback\n");
 
	GError *error = NULL;
	GVariant *result;
	gint response = 0;

	result = g_dbus_proxy_call_finish (proxy_log, res, &error);
	g_variant_get (result, "(i)", &response);
	g_print ("Int from Server is: %d \n", response);
}

static gboolean handle_signal_test(Log* skeleton,
        const gchar *arg_data,
        gpointer user_data)
{
	g_printf("signalCallback: %s\n", arg_data);
}

static void on_name_appeared (GDBusConnection *connection,
                  const gchar     *name,
                  const gchar     *name_owner,
                  gpointer         user_data)
{
    g_print("on_name_appeared: name %s, owner %s\n", name, name_owner);
    //
    GError *error = NULL;
    //
    proxy_log = g_dbus_proxy_new_sync(connection,
        G_DBUS_PROXY_FLAGS_NONE, 
        NULL, 
        "org.zhd.qbox", 
        "/org/zhd/qbox/log", 
        "org.zhd.qbox.log", 
        NULL, 
        NULL);
    // proxy_general = g_dbus_proxy_new_sync(connection,
    //     G_DBUS_PROXY_FLAGS_NONE, 
    //     NULL, 
    //     "org.zhd.qbox", 
    //     "/org/zhd/qbox/general_api", 
    //     "org.zhd.qbox.general_api", 
    //     NULL, 
    //     NULL);
    proxy_general = general_api_proxy_new_for_bus_sync (
            G_BUS_TYPE_SESSION,
            G_DBUS_PROXY_FLAGS_NONE,
            "org.zhd.qbox", 
            "/org/zhd/qbox/general", 
            NULL,
            &error);
    //
    // g_signal_connect(proxy_log, "signal-test", G_CALLBACK(handle_signal_test), NULL);
    //
    g_dbus_proxy_call(proxy_log, 
        "WriteData", 
        g_variant_new ("(s)", "1241435"), 
        G_DBUS_CALL_FLAGS_NONE, 
        -1, 
        NULL, 
        (GAsyncReadyCallback)methodCallback, 
        NULL);
    //
    // g_signal_connect(proxy_general, "client-request", G_CALLBACK(handle_client_request), NULL);
    //
    // const gchar value[3] = "123";
    typedef struct ttt{
        int i;
        char a[4];
    };
    struct ttt value = {
        .i = 0x12345678,
        .a = "1234"
    };
    value.a[1] = 0;
    gint out_output_int = 0, out_result = 0;
    //
    GVariant *va, *vb;
    var_enpkg(&va, &value, sizeof(value));
    //
    general_api_call_client_request_sync (
        proxy_general, 100, sizeof(value), va, 
        &out_output_int, &vb, &out_result, NULL, NULL);
    //
    struct ttt val;
    def_depkg(vb, &val, sizeof(val));
    printf("i = 0x%X, a = %s\n", val.i, val.a);
    int i = 0;
    for(i = 0; i < 4; i++)
        printf("[%d]: 0x%X\n", i, val.a[i]);
    //
    // g_variant_unref(va);
    // g_variant_unref(vb);
    //
    // g_dbus_proxy_call(proxy_general, 
    //     "ClientRequest", 
    //     // g_variant_new ("i", 100), 
    //     // g_variant_new ("i", 3), 
    //     g_variant_new ("(ii^ay)", 100, 3, "123"), 
    //     G_DBUS_CALL_FLAGS_NONE, 
    //     -1, 
    //     NULL, 
    //     NULL, 
    //     NULL);
}

static void on_name_vanished (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    g_print("on_name_vanished: name %s\n", name);
}

int main (int argc, char *argv[])
{
  guint watcher_id;
  GMainLoop *loop = NULL;

  watcher_id = g_bus_watch_name (
            G_BUS_TYPE_SESSION,
            "org.zhd.qbox",
            G_BUS_NAME_OWNER_FLAGS_NONE,
            on_name_appeared,
            on_name_vanished,
            NULL,
            NULL);

  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  g_bus_unwatch_name (watcher_id);
  return 0;
}

// int main (int argc, char **argv)
// {
// 	GDBusConnection *bcon;
// 	GMainLoop *loop;
 
// 	g_type_init ();
// 	bcon = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, NULL);
// 	proxy_log = g_dbus_proxy_new_sync (bcon, 
//         G_DBUS_PROXY_FLAGS_NONE, 
//         NULL, 
//         "org.zhd.qbox", 
//         "/org/zhd/qbox/log",
//         "org.zhd.qbox.log", 
//         NULL, 
//         NULL);
// 	g_dbus_proxy_call (proxy_log, 
//         "WriteData", 
//         g_variant_new ("(s)", "1231415"), 
//         G_DBUS_CALL_FLAGS_NONE, 
//         -1, 
//         NULL, 
//         (GAsyncReadyCallback) mycallback, 
//         NULL);
 
// 	loop = g_main_loop_new(NULL, FALSE);
// 	g_main_loop_run(loop);
// 	return 0;
// }
