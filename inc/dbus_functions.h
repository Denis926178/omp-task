#ifndef __DBUS_FUNCTIONS_H__
#define __DBUS_FUNCTIONS_H__

#include <stdio.h>
#include <unistd.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>

#define ERROR_CONNECTION        10

void send_signal(DBusConnection *connection, char *path, char *interface, char *signal, char *filename);
DBusConnection *init_bus(char *name);

#endif