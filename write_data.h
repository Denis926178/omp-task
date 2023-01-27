#ifndef WRITE_DATA_H
#define WRITE_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_STRING              256

void delete_space(char *string);
int read_field(char *string);
int write_app_info(char *name_in_dbus, char *way_to_start, char *expansions);
int get_app_info(void);

#endif
