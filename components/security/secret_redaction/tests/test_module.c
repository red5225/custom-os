#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_secret_redaction_name(), "secret_redaction") == 0);
    assert(customos_secret_redaction_capabilities() == 99UL);
    assert(customos_secret_redaction_self_check() == 0);
    assert(customos_secret_redaction_init() == 0);
    assert(customos_secret_redaction_self_check() == 1);
    return 0;
}
