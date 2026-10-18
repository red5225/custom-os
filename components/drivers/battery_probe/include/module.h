#ifndef CUSTOMOS_DRIVERS_BATTERY_PROBE_H
#define CUSTOMOS_DRIVERS_BATTERY_PROBE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_battery_probe_init(void);
unsigned long customos_battery_probe_capabilities(void);
const char *customos_battery_probe_name(void);
int customos_battery_probe_self_check(void);

#endif
