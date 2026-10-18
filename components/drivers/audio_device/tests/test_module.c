#include "module.h"
#include <assert.h>
#include <string.h>

int main(void) {
    assert(strcmp(customos_audio_device_name(), "audio_device") == 0);
    assert(customos_audio_device_capabilities() == 56UL);
    assert(customos_audio_device_self_check() == 0);
    assert(customos_audio_device_init() == 0);
    assert(customos_audio_device_self_check() == 1);
    return 0;
}
