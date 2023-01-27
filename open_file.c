#include <unistd.h>
#include <limits.h>

#include "read_data.h"

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
    char name_in_dbus[LEN_STRING], way_to_exec[LEN_STRING], expansions_string[LEN_STRING];
    char path_name[PATH_MAX];
    char *filename = argv[INDEX_FILE_NAME];
    char expansion[LEN_EXPANSION];

    if (is_file_valid(filename))
        ERROR(1, "Ошибка, невозможно открыть данный файл");
    
    get_expansion(filename, expansion);
    char *pn = getcwd(path_name, PATH_MAX);

    if (pn == NULL)
        ERROR(2, "Ошибка, невозможно определить директорию");

    strcat(path_name, "/");
    strcat(path_name, filename);

    FILE *f = fopen("./data/base.txt", "r");

    while (!get_one_record(f, name_in_dbus, way_to_exec, expansions_string))
        if (!is_expansion_in_string(expansions_string, expansion))
            printf("%s %s %s\n", name_in_dbus, way_to_exec, expansions_string);
        

    return EXIT_SUCCESS;
}
