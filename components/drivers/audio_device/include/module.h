#ifndef CUSTOMOS_DRIVERS_AUDIO_DEVICE_H
#define CUSTOMOS_DRIVERS_AUDIO_DEVICE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_audio_device_init(void);
unsigned long customos_audio_device_capabilities(void);
const char *customos_audio_device_name(void);
int customos_audio_device_self_check(void);

#endif
