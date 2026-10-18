#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_file_metadata_name(), "file_metadata") == 0);
    assert(customos_file_metadata_capabilities() == 44UL);
    assert(customos_file_metadata_self_check() == 0);
    assert(customos_file_metadata_init() == 0);
    assert(customos_file_metadata_self_check() == 1);
    return 0;
}
