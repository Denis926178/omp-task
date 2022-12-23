#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>

// DBusConnection *init_bus()
// {
//   DBusGConnection *connection;
//   DBusGError *err;
//   int ret;

//   dbus_error_init(err);
//   // Соединяем с dbus, устанавливаем соединение, называемое объектом

//   connection = dbus__g_bus_get(DBUS_BUS_SESSION, err);

//   if (dbus_error_is_set(&err))
//   {
//     printf("connection error: :%s -- %s\n", err.name, err.message);
//     dbus_error_free(err);
//     return NULL;
//   }
//   /// Назначаем имя этому объекту
//   ret = dbus_g_bus_request_name(connection, "hello.world.client", DBUS_NAME_FLAG_REPLACE_EXISTING, err);

//   if (dbus_error_is_set(err))
//   {
//     printf("Name error: %s -- %s\n", err.name, err.message);
//     dbus_error_free(rr);
//     return NULL;
//   }
//   if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
//     return NULL;

//   return connection;
// }

void send_signal(DBusConnection *connection)
{
  DBusMessage *msg;
  DBusMessageIter arg;
  char *str = "hello world!";

  // Создать объект сигнала
  // param1: path (в этой логике это может быть любая строка, если она соответствует правилам)
  // param2: interface (то же самое)
  // param3: имя метода сигнала (должно соответствовать имени сервера)
  if ((msg = dbus_message_new_signal("/hello", "aa.bb.cc", "alarm_test")) == NULL)
  {
    printf("message is NULL\n");
    return;
  }
#if 0
	 // Это зависит от необходимости добавления. Вообще говоря, сигнал является односторонней передачей, плюс это предложение становится односторонним одноадресным
	 //param2: bus_name
	if(!dbus_message_set_destination(msg, "hello.world.service"))
        {
                printf("memory error\n");
        }
#endif

  // Некоторые интерфейсы для добавления параметров
  dbus_message_iter_init_append(msg, &arg);
  dbus_message_iter_append_basic(&arg, DBUS_TYPE_STRING, &str);
  // Присоединиться к команде
  dbus_connection_send(connection, msg, NULL);
  // Послать
  dbus_connection_flush(connection);
  // Освободить память
  dbus_message_unref(msg);

  return;
}

void send_method_call(DBusConnection *connection)
{
  DBusMessage *msg;
  DBusMessageIter arg;
  DBusPendingCall *pending;
  int a = 100;
  int b = 99;
  int sum;

  msg = dbus_message_new_method_call("hello.world.service", "/hello/world", "hello.world", "add");
  if (msg == NULL)
  {
    printf("no memory\n");
    return;
  }

  dbus_message_iter_init_append(msg, &arg);
  if (!dbus_message_iter_append_basic(&arg, DBUS_TYPE_INT32, &a))
  {
    printf("no memory!");
    dbus_message_unref(msg);
    return;
  }
  if (!dbus_message_iter_append_basic(&arg, DBUS_TYPE_INT32, &b))
  {
    printf("no memory!");
    dbus_message_unref(msg);
    return;
  }

  // Входящее сообщение, ожидая ответа
  // param1: дескриптор соединения
  // param2:　message
  //  param3: эквивалент дескриптора обратного вызова для получения возвращенного сообщения
  //  param4: сверхурочные. -1 представляет бесконечность
  if (!dbus_connection_send_with_reply(connection, msg, &pending, -1))
  {
    printf("no memeory!");
    dbus_message_unref(msg);
    return;
  }

  if (pending == NULL)
  {
    printf("Pending is NULL, may be disconnect...\n");
    dbus_message_unref(msg);
    return;
  }
  // send
  dbus_connection_flush(connection);
  dbus_message_unref(msg);

  // Блокировать до получения ответа.
  dbus_pending_call_block(pending);
  msg = dbus_pending_call_steal_reply(pending);
  if (msg == NULL)
  {
    printf("reply is null. error\n");
    return;
  }
  // Свободная память в ожидании
  dbus_pending_call_unref(pending);
  // Параметры разбора
  if (!dbus_message_iter_init(msg, &arg))
    printf("no argument, error\n");
  if (dbus_message_iter_get_arg_type(&arg) != DBUS_TYPE_INT32)
  {
    printf("paramter type error\n");
  }

  dbus_message_iter_get_basic(&arg, &sum);

  printf(" a(%d) + b(%d) = %d\n", a, b, sum);
  dbus_message_unref(msg);

  return;
}

int main(void)
{
  DBusConnection *connection;
  DBusGConnection *conn;
  GError *error;
  DBusError err;
  DBusGProxy *proxy;
  char **name_list;
  char **name_list_ptr;

  // g_type_init();

  error = NULL;
  connection = dbus_bus_get(DBUS_BUS_SESSION, &err);

  if (connection == NULL)
  {
    g_printerr("Failed to open connection to bus: %s\n",
               error->message);
    g_error_free(error);
    exit(1);
  }

  int ret = dbus_bus_request_name(connection, "hellooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo.world.client", DBUS_NAME_FLAG_DO_NOT_QUEUE, &err);

  if (dbus_error_is_set(&err))
  {
    printf("Name error: %s -- %s\n", err.name, err.message);
    dbus_error_free(&err);
    return 2;
  }
  printf("%d %d\n", ret, DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER);
  
  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
    return 2;
  if (connection == NULL)
  {
    g_printerr("Failed to open connection to bus: %s\n",
               error->message);
    g_error_free(error);
    exit(1);
  }

  /* Create a proxy object for the "bus driver" (name "org.freedesktop.DBus") */

  //g_type_init();

  error = NULL;
  conn = dbus_g_bus_get (DBUS_BUS_SESSION,
                           &error);
  if (conn == NULL)
    {
      g_printerr ("Failed to open connection to bus: %s\n",
              error->message);
      g_error_free (error);
      exit (1);
    }

  proxy = dbus_g_proxy_new_for_name(conn,
                                    DBUS_SERVICE_DBUS,
                                    DBUS_PATH_DBUS,
                                    DBUS_INTERFACE_DBUS);

  /* Call ListNames method, wait for reply */
  error = NULL;
  if (!dbus_g_proxy_call(proxy, "ListNames", &error, G_TYPE_INVALID,
                         G_TYPE_STRV, &name_list, G_TYPE_INVALID))
  {
    /* Just do demonstrate remote exceptions versus regular GError */
    if (error->domain == DBUS_GERROR && error->code == DBUS_GERROR_REMOTE_EXCEPTION)
      g_printerr("Caught remote method exception %s: %s",
                 dbus_g_error_get_name(error),
                 error->message);
    else
      g_printerr("Error: %s\n", error->message);
    g_error_free(error);
    exit(1);
  }

  /* Print the results */

  g_print("Names on the message bus:\n");

  for (name_list_ptr = name_list; *name_list_ptr; name_list_ptr++)
  {
    g_print("  %s\n", *name_list_ptr);
  }
  g_strfreev(name_list);

  g_object_unref(proxy);

  return 0;
}