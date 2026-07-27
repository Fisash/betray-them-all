#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "input.h"

#define ERASE_CHAR_SEQUENCE "\b \b"

static int is_printable(int ch)
{
    return (ch > 31) && (ch < 127);
}

enum { escape=27, backspace=8, enter=10 };

void input_line(char *buf, size_t len)
{
    int is_done, ch;
    size_t cur;
    cur = 0;
    is_done = 0;
    while(!is_done)
    {
        ch = getchar();
        if(is_printable(ch) && cur<len-1)
        {
            buf[cur] = (char)ch;
            putchar(ch);
            cur++;
        }
        else if(ch == '\n')
        {
            putchar('\n');
            is_done = 1;
        }
        else if(ch == '\b' && cur > 0)
        {
            fputs(ERASE_CHAR_SEQUENCE, stdout);
            cur--;
        }
    }
    buf[cur] = '\0';
}

void input_line_prefix(char *buf, size_t len, const char *prefix)
{
    if (prefix)
        fputs(prefix, stdout);
    input_line(buf, len);
}

/*-----------------------------------------------------------------------*/

#if 0
enum { input_size=16 };

void simple_shell(const char *prefix)
{
    char input[input_size];
    int is_continue;
    is_continue = 1;
    while(is_continue)
    {
        if(prefix)
            fputs(prefix, stdout);
        input_line(input, input_size);
        if(strcmp(input, "quit") == 0)
            is_continue = 0;
        else if(strcmp(input, "q") == 0)
            is_continue = 0;
        else if(strlen(input))
            puts(input);
    }
}
#endif

/*-----------------------------------------------------------------------*/

int input_choose_number(int min, int max)
{
    char input[4];
    int res = 0;
    while(res < min || res > max)
    {
        input_line(input, 4);
        res = atoi(input);
    }
    return res;
}
