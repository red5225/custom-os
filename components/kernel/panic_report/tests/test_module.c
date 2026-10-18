#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_panic_report_name(), "panic_report") == 0);
    assert(customos_panic_report_capabilities() == 12UL);
    assert(customos_panic_report_self_check() == 0);
    assert(customos_panic_report_init() == 0);
    assert(customos_panic_report_self_check() == 1);
    return 0;
}
