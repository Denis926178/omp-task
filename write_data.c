 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "write_data.h"

#define READ_STRING             1
#define LEN_STRING              256

#define CHAR_SPACE              ' '

#define ERROR_READ_APP_INFO     1
#define ERROR_OPEN_FILE         2
#define ERROR_CLOSE_FILE        3

void delete_space(char *string)
{
    int j = 0;

    for (int i = 0; i < LEN_STRING; i++)
    {
        if (string[i] == CHAR_SPACE && (j == 0 || string[i + 1] == CHAR_SPACE))
            continue;

        string[j++] = string[i];
    }

    if(string[strlen(string) - 1] == ' ')
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

int write_app_info(char *name_in_dbus, char *way_to_start, char *expansions)
{
    FILE *f = fopen("./data.txt", "a+");

    if (f == NULL)
        return ERROR_OPEN_FILE;

    fprintf(f, "%s %s %s\n", name_in_dbus, way_to_start, expansions);

    if (fclose(f) == EOF)
        return ERROR_CLOSE_FILE;
    
    return EXIT_SUCCESS;
}

int get_app_info(void)
{
    char name_in_dbus[LEN_STRING], way_to_start[LEN_STRING], expansions[LEN_STRING];
    int code_return;

    printf("Введите название приложение: ");
    if ((code_return = read_field(name_in_dbus)))
        return code_return;

    printf("Введите способ для запуска приложения: ");
    if ((code_return = read_field(way_to_start)))
        return code_return;

    printf("Введите расширения файлов, которые может обрабатывать приложения (через пробел): ");
    if ((code_return = read_field(expansions)))
        return code_return;

    if ((code_return = write_app_info(name_in_dbus, way_to_start, expansions)))
        return code_return;

    return EXIT_SUCCESS;
}
