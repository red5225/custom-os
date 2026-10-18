#ifndef CUSTOMOS_TERMINAL_PROMPT_BUILDER_H
#define CUSTOMOS_TERMINAL_PROMPT_BUILDER_H

/* CustomOS component catalog API. This module is independently tested. */
int customos_prompt_builder_init(void);
unsigned long customos_prompt_builder_capabilities(void);
const char *customos_prompt_builder_name(void);
int customos_prompt_builder_self_check(void);

#endif
