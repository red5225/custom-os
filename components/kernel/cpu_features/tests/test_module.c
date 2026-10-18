#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_cpu_features_name(), "cpu_features") == 0);
    assert(customos_cpu_features_capabilities() == 14UL);
    assert(customos_cpu_features_self_check() == 0);
    assert(customos_cpu_features_init() == 0);
    assert(customos_cpu_features_self_check() == 1);
    return 0;
}
