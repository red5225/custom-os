#ifndef MOUSE_H
#define MOUSE_H
void mouse_init(void);
int mouse_poll(int *dx, int *dy, int *buttons);
#endif
