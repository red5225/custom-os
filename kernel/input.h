#ifndef INPUT_H
#define INPUT_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void input_init(void);
int keyboard_read(char *out);
int mouse_read(int *dx, int *dy, int *left, int *right, int *middle);

#endif
