#ifndef MOUSE_H
#define MOUSE_H
#include <stdint.h>
typedef struct { int16_t dx; int16_t dy; uint8_t buttons; } mouse_event_t;
void mouse_init(void);
int mouse_poll(mouse_event_t *event); /* 1 for a complete packet, 0 otherwise */
#endif
