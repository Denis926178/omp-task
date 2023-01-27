#include "dbus_functions.h"

#define ERROR_MEMORY(print_msg, msg) { printf("%s\n", print_msg); dbus_message_unref(msg); return; }
#define ERROR(text_error, name_error, error_msg) { printf("%s%s -- %s\n", text_error, name_error, error_msg); dbus_error_free(&error); return NULL; }

void send_signal(DBusConnection *connection, char *path, char *interface, char *signal, char *filename)
{
    DBusMessage *msg;
    DBusMessageIter arg;

    if ((msg = dbus_message_new_signal(path, interface, signal)) == NULL)
    {
        printf("Сообщение - NULL\n");
        return;
    }

    dbus_message_iter_init_append(msg, &arg);
    
    if (!dbus_message_iter_append_basic(&arg, DBUS_TYPE_STRING, filename))
        ERROR_MEMORY("Ошибка при выделении памяти", msg);
        
    dbus_connection_send(connection, msg, NULL);
    dbus_connection_flush(connection);
    dbus_message_unref(msg);

    printf("Файл был передан приложению\n");
    return;
}

DBusConnection *init_bus(char *name)
{
    DBusConnection *connection;
    DBusError error;
    int ret;

    dbus_error_init(&error);

    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);

    if (dbus_error_is_set(&error))
        ERROR("Ошибка подключения:", error.name, error.message);
  
    ret = dbus_bus_request_name(connection, name, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

    if (dbus_error_is_set(&error))
        ERROR("Ошибка имени:", error.name, error.message);

    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
        return NULL;

    return connection;
}