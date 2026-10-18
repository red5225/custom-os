#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_allocation_trace_name(), "allocation_trace") == 0);
    assert(customos_allocation_trace_capabilities() == 26UL);
    assert(customos_allocation_trace_self_check() == 0);
    assert(customos_allocation_trace_init() == 0);
    assert(customos_allocation_trace_self_check() == 1);
    return 0;
}
