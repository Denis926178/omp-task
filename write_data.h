#ifndef WRITE_DATA_H
#define WRITE_DATA_H

void delete_space(char *string);
int read_field(char *string);
int write_app_info(char *name_in_dbus, char *way_to_start, char *expansions);
int get_app_info(void);

#endif
