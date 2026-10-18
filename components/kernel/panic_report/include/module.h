#ifndef CUSTOMOS_KERNEL_PANIC_REPORT_H
#define CUSTOMOS_KERNEL_PANIC_REPORT_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_panic_report_init(void);
unsigned long customos_panic_report_capabilities(void);
const char *customos_panic_report_name(void);
int customos_panic_report_self_check(void);

#endif
