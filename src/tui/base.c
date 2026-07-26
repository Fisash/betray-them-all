#include <string.h>
#include <stdio.h>

#include "cli/base.h"

int cli_base_choose_number(uint8_t min, uint8_t max)
{
    int input;
    for(;;)
    {
        if(scanf("%d", &input) == 1)
        {
            if(input >= min && input <= max)
                return input;
        }
        else
        {
            while(getc(stdin) != '\n')
                ;
        }
    }
}
