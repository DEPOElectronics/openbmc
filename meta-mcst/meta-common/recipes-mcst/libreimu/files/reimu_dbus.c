#include "reimu.h"
#include <dbus/dbus.h>

DBusConnection *reimu_s_dbus_conn = NULL;
DBusMessage *reimu_s_dbus_msg = NULL;

int reimu_is_atexit_dbus_fini = 0;
static void reimu_dbus_fini(void)
{
    if (reimu_s_dbus_conn) dbus_connection_unref(reimu_s_dbus_conn);
    reimu_s_dbus_conn = NULL;
}

int reimu_is_atexit_dbus_msg_fini = 0;
static void reimu_dbus_msg_fini(void)
{
    if (reimu_s_dbus_msg) dbus_message_unref(reimu_s_dbus_msg);
    reimu_s_dbus_msg = NULL;
}

static void reimu_init_dbus(void)
{
    reimu_set_atexit(reimu_is_atexit_dbus_fini, reimu_dbus_fini);
    reimu_set_atexit(reimu_is_atexit_dbus_msg_fini, reimu_dbus_msg_fini);
}

void reimu_dbus_manage_service(const char *service, const char *command)
{
    DBusError dbus_error;
    reimu_init_dbus();

    reimu_dbus_fini();
    dbus_error_init(&dbus_error);
    if ((reimu_s_dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error)) == NULL) return;

    reimu_dbus_msg_fini();
    if((reimu_s_dbus_msg = dbus_message_new_method_call("org.freedesktop.systemd1", service, "org.freedesktop.systemd1.Unit", command)) == NULL) return;
    reimu_set_atexit(reimu_is_atexit_dbus_msg_fini, reimu_dbus_msg_fini);

    const char *job_type = "replace";
    if(!dbus_message_append_args(reimu_s_dbus_msg, DBUS_TYPE_STRING, &job_type, DBUS_TYPE_INVALID)) return;
    if(!dbus_connection_send(reimu_s_dbus_conn, reimu_s_dbus_msg, NULL)) return;

    reimu_dbus_msg_fini();
    reimu_dbus_fini();
}

int reimu_dbus_set_property_str(const char *service, const char *object, const char *interface, const char *property, const char *value)
{
    DBusError dbus_error;
    reimu_init_dbus();

    reimu_dbus_fini();
    dbus_error_init(&dbus_error);
    if ((reimu_s_dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error)) == NULL) return 22;

    reimu_dbus_msg_fini();
    if((reimu_s_dbus_msg = dbus_message_new_method_call(service, object, "org.freedesktop.DBus.Properties", "Set")) == NULL) return 20;
    reimu_set_atexit(reimu_is_atexit_dbus_msg_fini, reimu_dbus_msg_fini);

    if(!dbus_message_append_args(reimu_s_dbus_msg, DBUS_TYPE_STRING, &interface,  DBUS_TYPE_STRING, &property, DBUS_TYPE_INVALID)) return 21;

    DBusMessageIter dbus_iter, dbus_subiter;
    dbus_message_iter_init_append (reimu_s_dbus_msg, &dbus_iter);
    if (!dbus_message_iter_open_container (&dbus_iter, DBUS_TYPE_VARIANT, "s", &dbus_subiter)) return 23;
    if (!dbus_message_iter_append_basic(&dbus_subiter, DBUS_TYPE_STRING, &value)) { dbus_message_iter_abandon_container(&dbus_iter, &dbus_subiter); return 24; }
    if (!dbus_message_iter_close_container(&dbus_iter, &dbus_subiter)) return 25;

    if(!dbus_connection_send(reimu_s_dbus_conn, reimu_s_dbus_msg, NULL)) return 26;

    reimu_dbus_msg_fini();
    reimu_dbus_fini();
    return 0;
}
