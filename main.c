#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus.h>
 
DBusConnection* init_bus()
{
	DBusConnection *connection;
	DBusError err;
	int ret;
 
	dbus_error_init(&err);
 
	connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
	if(dbus_error_is_set(&err))
	{
		printf("connection error: :%s -- %s\n", err.name, err.message);
		dbus_error_free(&err);
		return NULL;
	}
 
	ret = dbus_bus_request_name(connection, "hello.world.client", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
  int ret_2 = dbus_bus_request_name(connection, "hello.worldddd.cliend", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	if(dbus_error_is_set(&err))
	{
		printf("Name error: %s -- %s\n", err.name, err.message);
		dbus_error_free(&err);
		return NULL;
	}
	if(ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
		return NULL;
 
	return connection;
}
 
 
void send_signal(DBusConnection *connection)
{
	DBusMessage *msg;
	DBusMessageIter arg;
	char *str = "hello world!";
 
 
	// Создаем объект сигнала
	// param1: path (в этой логике это может быть любая строка, если она соответствует правилам)
	// param2: interface (то же самое)
	// param3: имя метода сигнала (должно совпадать с именем сервера)
	if((msg = dbus_message_new_signal("/hello", "aa.bb.cc", "alarm_test")) == NULL)
	{
		printf("message is NULL\n");
		return;
	}
#if 0
	　// Это зависит от спроса. В общем, сигнал является односторонним, плюс это предложение становится односторонним одноадресным.
	 //param2: bus_name
	if(!dbus_message_set_destination(msg, "hello.world.service"))
        {
                printf("memory error\n");
        }
#endif
 
	// Добавляем несколько интерфейсов для параметров
	dbus_message_iter_init_append(msg, &arg);
	dbus_message_iter_append_basic(&arg, DBUS_TYPE_STRING, &str);
	//Вход
	dbus_connection_send(connection, msg, NULL);
	//Отправить
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
 
	msg = dbus_message_new_method_call("hello.world.service", "/hello/world","hello.world", "add");
	if(msg == NULL)
	{
		printf("no memory\n");
		return;
	}
 
	dbus_message_iter_init_append(msg, &arg);
    	if(!dbus_message_iter_append_basic (&arg, DBUS_TYPE_INT32,&a)){
        	printf("no memory!");
        	dbus_message_unref(msg);
        	return;
    	}
   	if(!dbus_message_iter_append_basic (&arg, DBUS_TYPE_INT32,&b)){
        	printf("no memory!");
        	dbus_message_unref(msg);
        	return;
    	}
 
    // Входное сообщение, ожидаем ответа
    // param1: дескриптор соединения
    //param2:　message
    // param3: дескриптор, эквивалентный обратному вызову, чтобы получить возвращенное сообщение
    // param4: Со временем. -1 означает неограниченный
    if(!dbus_connection_send_with_reply (connection, msg, &pending, -1)){
        printf("no memeory!");
        dbus_message_unref(msg);
        return;
    }
 
    if(pending == NULL){
        printf("Pending is NULL, may be disconnect...\n");
        dbus_message_unref(msg);
        return;
    }
    //send
    dbus_connection_flush(connection);
    dbus_message_unref(msg);
	
	// Блокировать до получения ответа.
    dbus_pending_call_block (pending);
    msg = dbus_pending_call_steal_reply (pending);
    if (msg == NULL) {
    	printf("reply is null. error\n");
    	return;
    }
    // Освобождаем ожидающую память 
    dbus_pending_call_unref(pending);
    // Анализируем параметры
    if (!dbus_message_iter_init(msg, &arg))
        printf("no argument, error\n");
    if(dbus_message_iter_get_arg_type(&arg) != DBUS_TYPE_INT32)
    {
    	printf("paramter type error\n");
    }
 
    dbus_message_iter_get_basic(&arg, &sum);
 
    printf(" a(%d) + b(%d) = %d\n",a, b, sum);
    dbus_message_unref(msg);
	
    return;
}
 
 
int main(int argc, char **argv)
{
	DBusConnection *connection;
 
	connection = init_bus();
	if(connection == NULL)
	{
		printf("connect to bus failed...\n");
		return -1;
	}
  while(1)
 {
	send_signal(connection);
	send_method_call(connection);
	sleep(5);
 }
	
	
	return 0;
}
