
#include <glib.h>
#include <gio/gio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "comm.h"
#include "test.h"

static Log* log_skeleton=NULL;
static General_api *general_api_skeleton = NULL;

static  gboolean handle_write_data(Log* object,
        GDBusMethodInvocation *invocation,
        const gchar *arg_data,
        gpointer user_data)
{
    int reply = 10086;
    printf("write_data: %s\n", arg_data);
    // reply = write_log(arg_data);

    log_complete_write_data(object,invocation,reply);

    return TRUE;
}

static  gboolean handle_set_attr(Log* object,
        GDBusMethodInvocation *invocation,
        const gchar *arg_attr_type,
        const gchar *arg_value,
        gpointer user_data)
{
    int reply = -1;
    g_print("Method call: attr_type is %d;value is %d\n", arg_attr_type, arg_value);
    //对于复杂数据结构可以使用variant方式进行序列化和反序列化操作
    // log_complete_set_attr(object,invocation,reply);
    
    return TRUE;
}

static  gboolean handle_client_request(General_api *object,
    GDBusMethodInvocation *invocation,
    gint arg_action_id,
    gint arg_input_int,
    GVariant *arg_in_arg)
{
    gint output_int;
    printf ("arg_action_id is:%d;arg_input_int is:%d\n", arg_action_id, arg_input_int);
    
    typedef struct ttt{
        int i;
        char a[4];
    };
    struct ttt val;
    def_depkg(arg_in_arg, &val, sizeof(val));
    printf("i = 0x%X, a = %s\n", val.i, val.a);
    int i = 0;
    for(i = 0; i < 4; i++)
        printf("[%d]: 0x%X\n", i, val.a[i]);

    GVariant *vb;
    val.i = 0x88776655;
    strcpy(val.a, "back");
    var_enpkg(&vb, &val, sizeof(val));
    general_api_complete_client_request(object, invocation, sizeof(val), vb, 10086);
    // g_variant_unref(vb);
    return TRUE;
}

gboolean emit_signal_test(gconstpointer p)          
{                                                         
    // g_print("Emit_Test_Status() is called.\n");            

    gchar *data = "signal test";
    log_emit_signal_test (log_skeleton, data);
    return TRUE;                                          
}  
gboolean emit_signal_request(gconstpointer p)          
{                                                         
    // g_print("Emit_Test_Status() is called.\n");            

    gchar *data = "signal test";
    general_api_emit_signal_request (general_api_skeleton, data);
    return TRUE;                                          
}
void GBusAcquired_Callback (GDBusConnection *connection,
        const gchar *name,
        gpointer user_data)
{
    GError *error = NULL;

    printf("GBusAcquired_Callback has been invoked\n");
    printf("GBusAcquired_Callback the name = %s\n",name);
    printf("GBusAcquired_Callback the user_data = %s\n",(char*)user_data);

    log_skeleton =  log_skeleton_new ();
    g_signal_connect(log_skeleton,"handle-write-data",G_CALLBACK(handle_write_data),NULL);
    g_signal_connect(log_skeleton,"handle-set-attr",G_CALLBACK(handle_set_attr),NULL);

    g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(log_skeleton), connection, "/org/zhd/qbox/log", &error);
    if(error != NULL){                                                           
        g_print("Error: Failed to export object. Reason: %s.\n", error->message);
        g_error_free(error);                                                     
    }

    general_api_skeleton = general_api_skeleton_new();
    g_signal_connect(general_api_skeleton,"handle-client-request",G_CALLBACK(handle_client_request),NULL);
    // g_signal_connect(general_api_skeleton,"handle-test-ay",G_CALLBACK(handle_test_ay),NULL);

    g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(general_api_skeleton), connection, "/org/zhd/qbox/general", &error);
    if(error != NULL){                                                           
        g_print("Error: Failed to export object. Reason: %s.\n", error->message);
        g_error_free(error);                                                     
    }                                                                           
}

void GBusNameAcquired_Callback (GDBusConnection *connection,
        const gchar *name,
        gpointer user_data)
{
    printf("GBusNameAcquired_Callback has been invoked\n");
}

void GBusNameLost_Callback (GDBusConnection *connection,
        const gchar *name,
        gpointer user_data)
{
    printf("GBusNameLost_Callback has been invoked\n");
}

//int gdbus_log_server_init()
int main(void)
{
    char* s = NULL;//"log";
    GMainLoop* loop = NULL;

    guint own_id = g_bus_own_name (
                G_BUS_TYPE_SESSION,
                "org.zhd.qbox",
                G_BUS_NAME_OWNER_FLAGS_NONE,
                GBusAcquired_Callback,
                GBusNameAcquired_Callback,
                GBusNameLost_Callback,
                (gpointer)s,
                NULL);
    g_timeout_add(1000, (GSourceFunc)emit_signal_request, NULL);
    g_timeout_add(1000, (GSourceFunc)emit_signal_test, NULL);
    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    g_bus_unown_name(own_id);
    return 0;
}
