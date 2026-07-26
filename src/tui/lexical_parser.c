#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cli/lexical_parser.h"

#define ARGS_SPLITER " \n"

static void parse_command(char *input_buf, struct command *cmd)
{
    cmd->argc = 0;
    char* current_token = strtok(input_buf, ARGS_SPLITER);
    while (current_token != NULL && cmd->argc < MAX_AGRC)
    {
        cmd->argv[cmd->argc] = current_token;
        cmd->argc++;
        current_token = strtok(NULL, ARGS_SPLITER);
    }
}

void lexical_command_input(struct command *cmd, char input_buffer[])
{
    do
    {
        memset(cmd, 0, sizeof(struct command));
        fgets(input_buffer, INPUT_BUF_SIZE, stdin);
        parse_command(input_buffer, cmd);
    }
    while (cmd->argc == 0);
}
