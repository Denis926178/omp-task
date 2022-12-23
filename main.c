#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>

#define ERROR_GET_CONNECTION    1
#define ERROR_GET_NAME          2
#define ERROR_GET_PROXY         3
#define ERROR_ARGC_NUMBER       4
#define ERROR_OPEN_FILE         5


// === THIS FUNCTION ADD INFO ABOUT APP INTO FILE ===


void add_data_to_file(const char *name_in_dbus, const char *way_to_exec, char *expansions)
{
  FILE *file_data = fopen("./data/base.txt", "a");

  if (file_data == NULL)
    return;
  
  fprintf(file_data, "%s %s %s\n", name_in_dbus, way_to_exec, expansions);

  fclose(file_data);
}


// === THIS FUNCTION ADD APP TO DBUS AND THEN WRITE INFO IN FILE ===


int add_app_in_dbus(const char *name_in_dbus, const char *way_to_exec, char *expansions)
{
  DBusConnection* connection;
  DBusError err;

  connection = dbus_bus_get(DBUS_BUS_SESSION, &err);

  if (connection == NULL)
  {
    printf("Failed to open connection to bus: %s\n", err.message);
    dbus_error_free(&err);
    
    return ERROR_GET_CONNECTION;
  }

  int ret = dbus_bus_request_name(connection, name_in_dbus, DBUS_NAME_FLAG_DO_NOT_QUEUE, &err);

  if (dbus_error_is_set(&err))
  {
    printf("Name error: %s -- %s\n", err.name, err.message);
    dbus_error_free(&err);

    return ERROR_GET_NAME;
  }

  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
    return ERROR_GET_NAME;

  if (connection == NULL)
  {
    printf("Failed to open connection to bus: %s\n", err.message);
    dbus_error_free(&err);

    return ERROR_GET_CONNECTION;
  }

  add_data_to_file(name_in_dbus, way_to_exec, expansions);

  return EXIT_SUCCESS;
}


// === IN MAIN FUNCTION YOU CAN SEE PROTOTYPE HOW THIS APP WORK OR OUTPUT IN CONSOLE APPS IN DBUS AFTER APP WITH EXPANSIONS HAS ADDED ===


int main(int argc, char **argv)
{
  if (argc == 2 && !strcmp(argv[1], "h"))
  {
    printf("Чтобы добавить в dbus приложение вводите параметры в таком таком виде:\n\"Название приложения на шине\"\
 \"Способ запуска данного приложения\" \"Расширения, которое может обрабатывать приложение\"\n ");
    printf("Названия расширений нужно вбивать через пробел, обязательно все расширения должны быть в двойных кавычках\n");
    printf("Пример aaa.ooo.ttt vim \"txt html py c\"\n");

    return 0;
  }

  if (argc <= 3)
    return ERROR_ARGC_NUMBER;

  DBusGConnection* conn;
  GError* error;
  DBusGProxy* proxy;
  char** name_list;
  char** name_list_ptr;
  int result;


  // HERE WE ADD OUR APP IN DBUS


  if ((result = add_app_in_dbus(argv[1], argv[2], argv[3])))
    return result;

  conn = dbus_g_bus_get(DBUS_BUS_SESSION, &error);

  if (conn == NULL)
  {
    g_printerr("Failed to open connection to bus: %s\n", error->message);
    g_error_free(error);

    return ERROR_GET_CONNECTION;
  }


  // HERE WE OUT APPS FROM DBUS

  
  proxy = dbus_g_proxy_new_for_name(conn,
    DBUS_SERVICE_DBUS,
    DBUS_PATH_DBUS,
    DBUS_INTERFACE_DBUS);

  error = NULL;

  if (!dbus_g_proxy_call(proxy, "ListNames", &error, G_TYPE_INVALID, G_TYPE_STRV, &name_list, G_TYPE_INVALID))
  {
    if (error->domain == DBUS_GERROR && error->code == DBUS_GERROR_REMOTE_EXCEPTION)
      g_printerr("Caught remote method exception %s: %s", dbus_g_error_get_name(error), error->message);
    else
      g_printerr("Error: %s\n", error->message);

    g_error_free(error);
    return ERROR_GET_PROXY;
  }

  g_print("Names on the message bus:\n");

  for (name_list_ptr = name_list; *name_list_ptr; name_list_ptr++)
  {
    g_print("  %s\n", *name_list_ptr);
  }

  g_strfreev(name_list);
  g_object_unref(proxy);

  return 0;
}