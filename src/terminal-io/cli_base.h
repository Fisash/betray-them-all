#ifndef CLI_BASE_H
#define CLI_BASE_H

#include <stdint.h>

#define INPUT_BUF_SIZE 256
#define MAX_AGRC 16

#define ARGS_SPLITER " \n"

typedef struct {
    uint8_t argc;
    char *argv [MAX_AGRC];
} command;

command cli_base_input_command(char input_buffer[]);

int cli_base_choose_number(uint8_t min, uint8_t max);

#endif
