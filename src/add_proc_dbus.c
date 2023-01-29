#include "dbus_functions.h"
#include "read_data.h"

#include <string.h>

#define NUMBER_APP      100

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

int is_app_reg(FILE *f, char *name_in_dbus)
{
    char temp[LEN_STRING];

    while (fscanf(f,"%s", temp) != EOF)
        if (!strcmp(temp, name_in_dbus))
            return EXIT_SUCCESS;
    
    return EXIT_FAILURE;
}

int main(void)
{
    DBusConnection *connection;

    char name_in_dbus[LEN_STRING];
    char path[LEN_STRING];
    char interface[LEN_STRING];
    char signal[LEN_STRING];
    char expansion_string[LEN_STRING];

    FILE *f = fopen("./data/base.txt", "r");

    if (f == NULL)
        return ERROR_OPEN_FILE;


    while (!get_one_record(f, name_in_dbus, path, interface, signal, expansion_string))
    {
        FILE *reg_app = fopen("./data/temp.txt", "r");

        if (reg_app == NULL)
        {
            fclose(f);
            return ERROR_OPEN_FILE;
        }

        if (!is_app_reg(reg_app, name_in_dbus))
        {
            fclose(reg_app);
            continue;
        }
        
        fclose(reg_app);

        connection = init_bus(name_in_dbus);
        
        reg_app = fopen("./data/temp.txt", "a+");

        if (reg_app == NULL)
        {
            fclose(f);
            return ERROR_OPEN_FILE;
        }

        if (connection)
            fprintf(reg_app, "%s\n", name_in_dbus);
        else
            printf("Не удалось установить соединение\n");
        
        fclose(reg_app);
    }

    fclose(f);
    int code_return = daemon_int();

    while (1)
        sleep(1);

    return code_return;
}
