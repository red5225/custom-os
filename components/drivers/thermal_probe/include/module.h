#ifndef CUSTOMOS_DRIVERS_THERMAL_PROBE_H
#define CUSTOMOS_DRIVERS_THERMAL_PROBE_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_thermal_probe_init(void);
unsigned long customos_thermal_probe_capabilities(void);
const char *customos_thermal_probe_name(void);
int customos_thermal_probe_self_check(void);

#endif
