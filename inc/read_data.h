#ifndef __READ_DATA_H__
#define __READ_DATA_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_EXPANSION           10
#define LEN_STRING              256

#define ERROR_OPEN_FILE         2

void get_expansion(char *filename, char *expansion);
int is_expansion_in_string(char *expansions_string, char *expansion);
int get_one_record(FILE *f, char *name_in_dbus, char *path, char *interface, char *signal, char *expansion_string);

#endif
