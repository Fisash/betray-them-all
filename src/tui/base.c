#include <string.h>
#include <stdio.h>

#include "tui/base.h"
#include "tui/print.h"

static void parse_command(char *input_buf, struct command *cmd)
{
    cmd->argc = 0;
    char* current_token = strtok(input_buf, ARGS_SPLITER);
    while (current_token != NULL && cmd->argc < MAX_AGRC)
    {
        cmd->argv[cmd->argc++] = current_token;
        current_token = strtok(NULL, ARGS_SPLITER);
    }
}

void cli_base_input_command(struct command *cmd, char input_buffer[])
{
    do
    {
        memset(cmd, 0, sizeof(struct command));
        fgets(input_buffer, INPUT_BUF_SIZE, stdin);
        parse_command(input_buffer, cmd);
    }
    while (cmd->argc == 0);
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
        {
            while(getc(stdin) != '\n')
                ;
        }
    }
}
