#include <string.h>
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
    memset(buf, 0, len);
    cur = 0;
    is_done = 0;
    while(!is_done && cur<len)
    {
        ch = getchar();
        if(is_printable(ch))
        {
            buf[cur] = (char)ch;
            putchar(ch);
            cur++;
        }
        else
        {
            switch(ch)
            {
                case enter:
                    putchar(ch);
                    is_done = 1;
                    break;
                case backspace:
                    if(cur > 0)
                    {
                        buf[cur] = '\0';
                        fputs(ERASE_CHAR_SEQUENCE, stdout);
                        cur--;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

enum { buf_size=16 };

void simple_shell(const char *prefix)
{
    char buf[buf_size];
    int is_continue;
    is_continue = 1;
    while(is_continue)
    {
        memset(buf, 0, buf_size);
        if (prefix)
            fputs(prefix, stdout);
        input_line(buf, buf_size);
        if (strcmp(buf, "quit") == 0)
            is_continue = 0;
    }
}

