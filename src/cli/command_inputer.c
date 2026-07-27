#include "cli/command_inputer.h"
#include <stdio.h>
#include <string.h>
#include "input.h"

void command_inputer_init(struct command_inputer *c)
{
    memset(c->input_buf, 0, INPUT_BUF_SIZE);
    commands_info_init(c->commands_info);
}

/* move to struct field ? */
#define INPUT_PREFIX " / "

void command_inputer_input(struct command_inputer *c)
{
    struct command lex_cmd;
    do
    {
        memset(&lex_cmd, 0, sizeof(struct command));  /* strange */
        input_line_prefix(c->input_buf, INPUT_BUF_SIZE, INPUT_PREFIX);
        command_parse_lexical(c->input_buf, &lex_cmd);
    }
    while(lex_cmd.argc == 0);
    command_parse_syntax(&c->cmd, &lex_cmd, c->commands_info);
}
