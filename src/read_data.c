#include "read_data.h"

#define READ_STRING         1

#define ENTER               '\n'
#define SPACE               ' '
#define ZERO_SYMB           '\0'
#define DOT                 '.'

#define ERROR_READ_DATA     4

void get_expansion(char *filename, char *expansion)
{
    if (*filename == DOT)
        filename++;
        
    for (; *filename!= ZERO_SYMB; filename++)
        if (*filename == DOT)
            break;
    
    int i = 0;
    filename++;

    for (; *filename != ZERO_SYMB; filename++)
        expansion[i++] = *filename;

    expansion[i] = ZERO_SYMB;
}

int is_expansion_in_string(char *expansion_string, char *expansion)
{
    char temp_expansion[LEN_EXPANSION];
    int i = 0;

    for (; *expansion_string != ZERO_SYMB; expansion_string++)
        if (*expansion_string != SPACE)
        {
            temp_expansion[i++] = *expansion_string;
        }
        else
        {
            temp_expansion[i] = ZERO_SYMB;
            if (!strcmp(expansion, temp_expansion))
                return EXIT_SUCCESS;
            else
                i = 0;
        }

    temp_expansion[i] = ZERO_SYMB;
    if (!strcmp(expansion, temp_expansion))
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

int get_one_record(FILE *f, char *name_in_dbus, char *path, char *interface, char *signal, char *expansion_string)
{
    if (fscanf(f, "%s ", name_in_dbus) != READ_STRING)
        return ERROR_READ_DATA;
    
    if (fscanf(f, "%s ", path) != READ_STRING)
        return ERROR_READ_DATA;
    
    if (fscanf(f, "%s ", interface) != READ_STRING)
        return ERROR_READ_DATA;
    
    if (fscanf(f, "%s ", signal) != READ_STRING)
        return ERROR_READ_DATA;
    
    if (fgets(expansion_string, LEN_STRING, f) == NULL)
        return ERROR_READ_DATA;

    if (expansion_string[strlen(expansion_string) - 1] == ENTER)
        expansion_string[strlen(expansion_string) - 1] = ZERO_SYMB;

    return EXIT_SUCCESS;
}
