#include "write_data.h"

int main(void)
{
    int code_return;

    if ((code_return = get_app_info()))
        return code_return;
    
    return EXIT_SUCCESS;
}
