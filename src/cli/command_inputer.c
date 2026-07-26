#include "cli/command_inputer.h"
#include <stdio.h>
#include <string.h>
#include "input.h"

#if 0
static void lexical_input(struct command *cmd, char input_buffer[])
{
    do
    {
        memset(cmd, 0, sizeof(struct command));
        fgets(input_buffer, INPUT_BUF_SIZE, stdin);
        command_parse_lexical(input_buffer, cmd);
    }
    while (cmd->argc == 0);
}
#endif

#define INPUT_PREFIX " / "

static void lexical_input(struct command *cmd, char input_buffer[])
{
    do
    {
        memset(cmd, 0, sizeof(struct command));

        fputs(INPUT_PREFIX, stdout);    /* temporary i guess */

        input_line(input_buffer, INPUT_BUF_SIZE);
        command_parse_lexical(input_buffer, cmd);
    }
    while(cmd->argc == 0);
}


void command_inputer_init(struct command_inputer *c)
{
    commands_info_init(c->commands_info);
}

void command_inputer_input(struct command_inputer *c)
{
    struct command lex_cmd;
    lexical_input(&lex_cmd, c->input_buf);
    command_parse_syntax(&c->cmd, &lex_cmd, c->commands_info);
}
