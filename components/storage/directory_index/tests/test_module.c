#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_directory_index_name(), "directory_index") == 0);
    assert(customos_directory_index_capabilities() == 45UL);
    assert(customos_directory_index_self_check() == 0);
    assert(customos_directory_index_init() == 0);
    assert(customos_directory_index_self_check() == 1);
    return 0;
}
