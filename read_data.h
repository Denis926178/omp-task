#ifndef READ_DATA_H
#define READ_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_EXPANSION           10
#define LEN_STRING              256

void get_expansion(char *filename, char *expansion);
int is_expansion_in_string(char *expansions_string, char *expansion);
int get_one_record(FILE *f, char *name_in_dbus, char *way_to_exec, char *expansions);
int count_records();

#endif
