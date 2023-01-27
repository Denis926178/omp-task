#ifndef __WRITE_DATA_H__
#define __WRITE_DATA_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_STRING              256

void delete_space(char *string);
int read_field(char *string);
int write_app_info(char *name_in_dbus, char *path, char *interface, char *signal, char *expansion_string);
int get_app_info(void);

#endif
