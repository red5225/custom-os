#ifndef MOUSE_H
#define MOUSE_H
#include <stdint.h>
void mouse_init(void);
void mouse_poll(void);
void mouse_isr_handler(void);
void mouse_get(int*x,int*y,uint8_t*buttons);
#endif
