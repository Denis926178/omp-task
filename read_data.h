#ifndef READ_DATA_H
#define READ_DATA_H

#define LEN_STRING              256

int is_expansion_in_string(char *expansions_string, char *expansion);
int get_one_record(FILE *f, char *name_in_dbus, char *way_to_exec, char *expansions);

#endif
