#include "xcl_runtime.h"
int xcl_streq(const char*a,const char*b){while(*a&&*b&&*a==*b){a++;b++;}return *a==*b;}
