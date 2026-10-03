#include "mouse.h"
void mouse_init(void){}
void mouse_poll(void){}
void mouse_isr_handler(void){}
void mouse_get(int*x,int*y,unsigned char*b){if(x)*x=0;if(y)*y=0;if(b)*b=0;}
