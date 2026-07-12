#include <stdio.h>
#include <string.h>

#include "terminal-io/cli_base.h"

static void parse_command(char *input_buf, command *cmd)
{
    cmd->argc = 0;
    char* current_token = strtok(input_buf, ARGS_SPLITER);
    while (current_token != NULL && cmd->argc < MAX_AGRC)
    {
        cmd->argv[cmd->argc++] = current_token;
        current_token = strtok(NULL, ARGS_SPLITER);
    }
}

command cli_base_input_command(char input_buffer[])
{
    command result;
    do
    {
        memset(&result, 0, sizeof(result));
        fgets(input_buffer, INPUT_BUF_SIZE, stdin);
        parse_command(input_buffer, &result);
    }
    while (result.argc == 0);

    return result;
}

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
            while(getc(stdin) != '\n');
    }
}

