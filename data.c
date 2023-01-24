#include <stdio.h>
#include <stdlib.h>

void add_data_to_file(char *name_in_bus, char *exec, char *expansion)
{
    FILE *f = fopen("./data/base.txt", "a");

    fprintf(f, "%s %s %s\n", *name_in_bus, *exec, *expansion);

    fclose(f);
}

void get_next_name_in_bus(FILE *f)
{
    
}

int main(void)
{
    return EXIT_SUCCESS;
}