#include "write_data.h"

#define READ_STRING             1

#define SPACE                   ' '

#define ERROR_READ_APP_INFO     1
#define ERROR_OPEN_FILE         2
#define ERROR_CLOSE_FILE        3

void delete_space(char *string)
{
    int j = 0;

    for (int i = 0; i < LEN_STRING; i++)
    {
        if (string[i] == SPACE && (j == 0 || string[i + 1] == SPACE))
            continue;

        string[j++] = string[i];
    }

    if(string[strlen(string) - 1] == SPACE)
        string[strlen(string) - 1] = '\0';
}

int read_field(char *string)
{
    if (fgets(string, LEN_STRING, stdin) == NULL)
        return ERROR_READ_APP_INFO;

    if ((strlen(string) > 0) && (string[strlen (string) - 1] == '\n'))
        string[strlen(string) - 1] = '\0';
    
    delete_space(string);

    return EXIT_SUCCESS;
}

int write_app_info(char *name_in_dbus, char *path, char *interface, char *signal, char *expansion_string)
{
    FILE *f = fopen("./data/base.txt", "a+");

    if (f == NULL)
        return ERROR_OPEN_FILE;

    fprintf(f, "%s %s %s %s %s\n", name_in_dbus, path, interface, signal, expansion_string);

    if (fclose(f) == EOF)
        return ERROR_CLOSE_FILE;
    
    return EXIT_SUCCESS;
}

int get_app_info(void)
{
    char name_in_dbus[LEN_STRING];
    char path[LEN_STRING];
    char interface[LEN_STRING];
    char signal[LEN_STRING];
    char expansion_string[LEN_STRING];

    int code_return;

    printf("Введите имя приложения в dbus: ");
    if ((code_return = read_field(name_in_dbus)))
        return code_return;

    printf("Введите путь к объекту в dbus: ");
    if ((code_return = read_field(path)))
        return code_return;

    printf("Введите имя интерфейса: ");
    if ((code_return = read_field(interface)))
        return code_return;
    
    printf("Введите называние сигнала, которое будет принимать приложение: ");
    if ((code_return = read_field(signal)))
        return code_return;

    printf("Введите расширения файлов, которые может обрабатывать приложения (через пробел): ");
    if ((code_return = read_field(expansion_string)))
        return code_return;

    if ((code_return = write_app_info(name_in_dbus, path, interface, signal, expansion_string)))
        return code_return;

    return EXIT_SUCCESS;
}
