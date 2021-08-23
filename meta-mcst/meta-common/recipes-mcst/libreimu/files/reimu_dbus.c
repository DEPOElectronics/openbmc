#include "reimu.h"
#include <dbus/dbus.h>

DBusConnection *reimu_dbus_conn = NULL;
DBusMessage *reimu_dbus_msg = NULL;
DBusMessage *reimu_dbus_reply = NULL;

int reimu_is_atexit_dbus_fini = 0;
static void reimu_dbus_fini(void)
{
    if (reimu_dbus_conn) dbus_connection_unref(reimu_dbus_conn);
    reimu_dbus_conn = NULL;
}

int reimu_is_atexit_dbus_msg_fini = 0;
static void reimu_dbus_msg_fini(void)
{
    if (reimu_dbus_msg) dbus_message_unref(reimu_dbus_msg);
    reimu_dbus_msg = NULL;
}

int reimu_is_atexit_dbus_reply_fini = 0;
static void reimu_dbus_reply_fini(void)
{
    if (reimu_dbus_reply) dbus_message_unref(reimu_dbus_reply);
    reimu_dbus_reply = NULL;
}

DBusError reimu_dbus_error;
static int reimu_init_dbus(const char *service, const char *object, const char *interface, const char *method)
{
    reimu_set_atexit(reimu_is_atexit_dbus_fini, reimu_dbus_fini);
    reimu_set_atexit(reimu_is_atexit_dbus_msg_fini, reimu_dbus_msg_fini);
    reimu_set_atexit(reimu_is_atexit_dbus_reply_fini, reimu_dbus_reply_fini);

    reimu_dbus_fini();
    dbus_error_init(&reimu_dbus_error);
    if ((reimu_dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, &reimu_dbus_error)) == NULL) return -1;

    reimu_dbus_msg_fini();
    return (reimu_dbus_msg = dbus_message_new_method_call(service, object, "org.freedesktop.DBus.Properties", "Set")) == NULL;
}

static int reimu_init_dbus_request(const char *string1, const char *string2)
{
    if (string2 == NULL)
        return !dbus_message_append_args(reimu_dbus_msg, DBUS_TYPE_STRING, &string1, DBUS_TYPE_INVALID);
    else
        return !dbus_message_append_args(reimu_dbus_msg, DBUS_TYPE_STRING, &string1, DBUS_TYPE_STRING, &string2, DBUS_TYPE_INVALID);
}

static int reimu_send_dbus_request(void)
{
    return !dbus_connection_send(reimu_dbus_conn, reimu_dbus_msg, NULL);
}

static int reimu_get_dbus_reply(void)
{
    reimu_dbus_reply_fini();
    return (reimu_dbus_reply = dbus_connection_send_with_reply_and_block(reimu_dbus_conn, reimu_dbus_msg, 1000, &reimu_dbus_error)) == NULL;
}

int reimu_dbus_set_property_str(const char *service, const char *object, const char *interface, const char *property, const char *value)
{
    if(reimu_init_dbus(service, object, "org.freedesktop.DBus.Properties", "Set")) return 20;
    if(reimu_init_dbus_request(interface, property)) return 21;

    DBusMessageIter dbus_iter, dbus_subiter;
    dbus_message_iter_init_append (reimu_dbus_msg, &dbus_iter);
    if (!dbus_message_iter_open_container (&dbus_iter, DBUS_TYPE_VARIANT, "s", &dbus_subiter)) return 23;
    if (!dbus_message_iter_append_basic(&dbus_subiter, DBUS_TYPE_STRING, &value)) { dbus_message_iter_abandon_container(&dbus_iter, &dbus_subiter); return 24; }
    if (!dbus_message_iter_close_container(&dbus_iter, &dbus_subiter)) return 25;

    return reimu_send_dbus_request() ? 26: 0;
}

int reimu_dbus_set_property_bool(const char *service, const char *object, const char *interface, const char *property, int value)
{
    if(reimu_init_dbus(service, object, "org.freedesktop.DBus.Properties", "Set")) return 20;
    if(reimu_init_dbus_request(interface, property)) return 21;

    DBusMessageIter dbus_iter, dbus_subiter;
    dbus_message_iter_init_append (reimu_dbus_msg, &dbus_iter);
    if (!dbus_message_iter_open_container (&dbus_iter, DBUS_TYPE_VARIANT, "b", &dbus_subiter)) return 23;
    if (!dbus_message_iter_append_basic(&dbus_subiter, DBUS_TYPE_BOOLEAN, &value)) { dbus_message_iter_abandon_container(&dbus_iter, &dbus_subiter); return 24; }
    if (!dbus_message_iter_close_container(&dbus_iter, &dbus_subiter)) return 25;

    return reimu_send_dbus_request() ? 26: 0;
}

int reimu_dbus_get_property_bool(const char *service, const char *object, const char *interface, const char *property)
{
    if(reimu_init_dbus(service, object, "org.freedesktop.DBus.Properties", "Get")) return 20;
    if(reimu_init_dbus_request(interface, property)) return 29;
    if(reimu_get_dbus_reply()) return 30;

    DBusBasicValue value;
    DBusMessageIter dbus_iter, dbus_subiter;
    if (!dbus_message_iter_init(reimu_dbus_reply, &dbus_iter)) return 31;
    if (dbus_message_iter_get_arg_type (&dbus_iter) != DBUS_TYPE_VARIANT) return 32;
    dbus_message_iter_recurse (&dbus_iter,&dbus_subiter);
    if (dbus_message_iter_get_arg_type (&dbus_subiter) != DBUS_TYPE_BOOLEAN) return 33;
    dbus_message_iter_get_basic(&dbus_subiter, &value);
    return (value.bool_val) ? 1: 0;
}

int reimu_dbus_call_method(const char *destination, const char *path, const char *iface, const char *method)
{
    if(reimu_init_dbus(destination, path, iface, method)) return 20;
    return reimu_get_dbus_reply() ? 34: 0;
}

void reimu_dbus_manage_service(const char *service, const char *command)
{
    if(reimu_init_dbus("org.freedesktop.systemd1", service, "org.freedesktop.systemd1.Unit", command)) return;
    if(reimu_init_dbus_request("replace", NULL)) return;
    reimu_send_dbus_request();
}
