#include "nol_runtime.h"
int nol_streq(const char *a,const char *b){
    while(*a && *b){ if(*a++ != *b++) return 0; }
    return *a == *b;
}
