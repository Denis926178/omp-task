#include <stdio.h>
#include <stdlib.h>

#include "write_data.c"

int main(void)
{
    int code_return;

    if ((code_return = get_app_info()))
        return code_return;
    
    return EXIT_SUCCESS;
}
