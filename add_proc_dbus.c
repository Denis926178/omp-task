#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus.h>

#include "read_data.h"
#include "write_data.h"

#define ERROR_CONNECTION        10

DBusConnection *init_bus(char *name)
{
    DBusConnection *connection;
    DBusError error;
    int ret;

    dbus_error_init(&error);

    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);

    if (dbus_error_is_set(&error))
    {
        printf("connection error: :%s -- %s\n", error.name, error.message);
        dbus_error_free(&error);
        return NULL;
    }

    ret = dbus_bus_request_name(connection, name, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

    if (dbus_error_is_set(&error))
    {
        printf("Name error: %s -- %s\n", error.name, error.message);
        dbus_error_free(&error);
        return NULL;
    }

    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
        return NULL;

    return connection;
}

int main(int argc, char **argv)
{
    FILE *f = fopen("./data/base.txt", "r");
    DBusConnection *connection;

    char name_in_dbus[LEN_STRING], way_to_exec[LEN_STRING], expansions[LEN_STRING];
    
    if (argc == 1)
    {
        while (!get_one_record(f, name_in_dbus, way_to_exec, expansions))
        {
            connection = init_bus(name_in_dbus);

            if (connection == NULL)
            {
                printf("Connect to bus failed...\n");
                return ERROR_CONNECTION;
            }
        }
    }

    int code_return;

    if (argc == 2 && !strcmp(argv[1], "add"))
    {
        if ((code_return = get_app_info()))
            return code_return;

        dbus_connection_dispatch(connection);
        while (!get_one_record(f, name_in_dbus, way_to_exec, expansions))
        {
            connection = init_bus(name_in_dbus);

            if (connection == NULL)
            {
                printf("Connect to bus failed...\n");
                return ERROR_CONNECTION;
            }
        }
    }   

    while(1)
    {
	    sleep(5);
    }

    return EXIT_SUCCESS;
}