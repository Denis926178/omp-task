#ifndef __DBUS_FUNCTIONS_H__
#define __DBUS_FUNCTIONS_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ERROR_CONNECTION        10

int send_signal(DBusConnection *connection, char *path, char *interface, char *signal, char *filename);
DBusConnection *init_bus(char *name);

#endif
