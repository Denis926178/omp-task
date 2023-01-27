#include "dbus_functions.h"
#include "read_data.h"

int daemon_int(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        return EXIT_FAILURE;
    else if (pid != 0)
       exit(0);

    setsid();  
    chdir("/");
    umask(0); 
    
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    DBusConnection *connection;

    char name_in_dbus[LEN_STRING];
    char path[LEN_STRING];
    char interface[LEN_STRING];
    char signal[LEN_STRING];
    char expansion_string[LEN_STRING];

    FILE *f = fopen("../data/base.txt", "r");

    if (f == NULL)
        return ERROR_OPEN_FILE;

    while (!get_one_record(f, name_in_dbus, path, interface, signal, expansion_string))
    {
        connection = init_bus(name_in_dbus);

        if (connection == NULL)
            printf("Connect to bus failed...\n");
    }

    fclose(f);
    daemon_int();

    return EXIT_SUCCESS;
}
