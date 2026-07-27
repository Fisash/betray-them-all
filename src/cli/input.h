#ifndef INPUT_H
#define INPUT_H

void input_line(char *buf, size_t len);
void input_line_prefix(char *buf, size_t len, const char *prefix);

int input_choose_number(int min, int max);

#endif
