#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_audit_events_name(), "audit_events") == 0);
    assert(customos_audit_events_capabilities() == 95UL);
    assert(customos_audit_events_self_check() == 0);
    assert(customos_audit_events_init() == 0);
    assert(customos_audit_events_self_check() == 1);
    return 0;
}
