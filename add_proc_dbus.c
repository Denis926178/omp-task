#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus.h>
#include <sys/types.h>
#include <sys/stat.h> 

#include "read_data.h"

#define ERROR_CONNECTION        10

int daemon_int(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        return EXIT_FAILURE;
    else if (pid != 0)
       exit(0);

    setsid();  
    chdir("/");
    umask(0); 
    
    return EXIT_SUCCESS;
}

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
    DBusConnection *connection;

    char name_in_dbus[LEN_STRING], way_to_exec[LEN_STRING], expansions[LEN_STRING];

    FILE *f = fopen("./data/base.txt", "r");
    while (!get_one_record(f, name_in_dbus, way_to_exec, expansions))
    {
        connection = init_bus(name_in_dbus);

        if (connection == NULL)
            printf("Connect to bus failed...\n");
    }

    fclose(f);
    daemon_int();

    return EXIT_SUCCESS;
}
