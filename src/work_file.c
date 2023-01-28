#define _POSIX_C_SOURCE 1
#include <unistd.h>
#include <limits.h>

#include "read_data.h"
#include "dbus_functions.h"

#define INDEX_FILE_NAME         1
#define ERROR(x, msg) { printf("%s\n", msg); return x; }

int is_file_valid(char *filename)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL)
        return EXIT_FAILURE;
    
    fclose(f);

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        ERROR(6, "Неверное количество файлов");

    char name_in_dbus[LEN_STRING];
    char path[LEN_STRING];
    char interface[LEN_STRING];
    char signal[LEN_STRING];
    char expansion_string[LEN_STRING];
    char path_name[PATH_MAX];
    char *filename = argv[INDEX_FILE_NAME];
    char expansion[LEN_EXPANSION];

    if (is_file_valid(filename))
        ERROR(1, "Ошибка, невозможно открыть данный файл");
    
    get_expansion(filename, expansion);
    char *pn = getcwd(path_name, PATH_MAX);

    if (pn == NULL)
        ERROR(5, "Ошибка, невозможно определить директорию");

    strcat(path_name, "/");
    strcat(path_name, filename);

    FILE *f = fopen("./data/base.txt", "r");

    if (f == NULL)
        ERROR(2, "Ошибка, невозможно посмотреть список приложений, работающих с данной утилитой");

    int counter = 0;
    DBusConnection *connection;

    while (!get_one_record(f, name_in_dbus, path, interface, signal, expansion_string))
        if (!is_expansion_in_string(expansion_string, expansion))
        {   
            connection = init_bus("helloo.world.client");

            if (connection == NULL)
                ERROR(ERROR_CONNECTION, "Невозможно подключиться к сессионной шине dbus");

            printf("    Название приложения в dbus: %s\n", name_in_dbus);
            printf("    Путь к объекту, зарегестрированному в dbus: %s\n", path);
            printf("    Интерфейс, работающий с файлом: %s\n", interface);
            printf("    Сигнал, отправляемый приложению, которое будет работать с файлом: %s\n", signal);
            printf("    Список расширений, с которыми данное приложение может работать: %s\n", expansion_string);
            printf("    Полное имя файла, с которым будет работать приложение: %s\n", path_name);

            int code_return = send_signal(connection, path, interface, signal, expansion_string);
            if (!code_return)
            {
                counter = 1;
                break;
            }
        }
    
    if (!counter)
        ERROR(7, "Нет приложения, которое может обработать данный файл");

    return EXIT_SUCCESS;
}
