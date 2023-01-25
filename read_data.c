#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "read_data.h"

#define READ_STRING         1
#define LEN_STRING          256
#define LEN_EXPANSION       10

#define ENTER               '\n'
#define SPACE               ' '
#define ZERO_SYMB           '\0'

#define ERROR_READ_DATA     4

int is_expansion_in_string(char *expansions_string, char *expansion)
{
    char temp_expansion[LEN_EXPANSION];
    int i = 0;

    while (*expansions_string != ZERO_SYMB)
    {
        if (*expansions_string != SPACE)
        {
            temp_expansion[i++] = *expansions_string;
        }
        else
        {
            temp_expansion[i] = '\0';
            if (strcmp(expansion, temp_expansion) == 0)
                return EXIT_SUCCESS;
            else
                i = 0;
        }
        expansions_string++;
    }

    temp_expansion[i] = '\0';
    if (strcmp(expansion, temp_expansion) == 0)
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

int get_one_record(FILE *f, char *name_in_dbus, char *way_to_exec, char *expansions)
{
    if (fscanf(f, "%s", name_in_dbus) != READ_STRING)
        return ERROR_READ_DATA;
    
    if (fscanf(f, "%s ", way_to_exec) != READ_STRING)
        return ERROR_READ_DATA;
    
    if (fgets(expansions, LEN_STRING, f) == NULL)
        return ERROR_READ_DATA;

    if (expansions[strlen(expansions) - 1] == ENTER)
        expansions[strlen(expansions) - 1] = ZERO_SYMB;

    return EXIT_SUCCESS;
}

// int main(int argc, char **argv)
// {
//     FILE *f = fopen("./data/base.txt", "r");

//     char name_in_dbus[LEN_STRING], way_to_exec[LEN_STRING], expansions_string[LEN_STRING];
//     char *expansion = argv[1];

//     while (!get_one_record(f, name_in_dbus, way_to_exec, expansions_string))
//     {
//         int rc = is_expansion_in_string(expansions_string, expansion);
//         printf("%s %s %s - %d\n", name_in_dbus, way_to_exec, expansions_string, rc);
//     }

//     fclose(f);
// }