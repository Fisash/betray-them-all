#ifndef COMMAND_INPUTER_H
#define COMMAND_INPUTER_H

#include "cli/command_info.h"
#include "cli/command_parser.h"

#define INPUT_BUF_SIZE 128

struct command_inputer {
    char input_buf[INPUT_BUF_SIZE];
    const struct command_info *commands_info[CMD_COUNT];

    struct parsed_command cmd;
};

void command_inputer_init(struct command_inputer *c);

void command_inputer_input(struct command_inputer *c);

#endif
