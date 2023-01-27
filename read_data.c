#include "read_data.h"

#define READ_STRING         1

#define ENTER               '\n'
#define SPACE               ' '
#define ZERO_SYMB           '\0'
#define DOT                 '.'

#define ERROR_READ_DATA     4

void get_expansion(char *filename, char *expansion)
{
    while (*filename!= ZERO_SYMB)
    {
        if (*filename == DOT)
            break;
        
        filename++;
    }
    
    int i = 0;
    filename++;

    while (*filename != ZERO_SYMB)
    {
        expansion[i++] = *filename;
        filename++;
    }

    expansion[i] = ZERO_SYMB;
}

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

    temp_expansion[i] = ZERO_SYMB;
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

int count_records()
{
    FILE *f = fopen("./data/base.txt", "r");
    int counter = 0;
    int ch;

    do
    {
        ch = fgetc(f);
        if(ch == ENTER) counter++;   
    } while( ch != EOF );    

    return counter;
}
