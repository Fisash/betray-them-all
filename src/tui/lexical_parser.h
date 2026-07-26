#ifndef LEXICAL_PARSER_H
#define LEXICAL_PARSER_H

#include <stdint.h>

#define MAX_AGRC 8
#define INPUT_BUF_SIZE 256

struct command {
    uint8_t argc;
    char *argv [MAX_AGRC];
};

void lexical_command_input(struct command *cmd, char input_buffer[]);

#endif
