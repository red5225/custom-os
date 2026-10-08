#include <stdint.h>
#include "limine.h"
#include "terminal.h"
#include "keyboard.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

extern void boot(void);
extern void command(const char *cmd);

static void halt_forever(void) {
    for (;;) __asm__ volatile ("hlt");
}

static void input_loop(void) {
    char line[128];
    unsigned int len = 0;

    for (;;) {
        int c = keyboard_getchar();
        if (c < 0) continue;
        if (c == 10) {
            line[len] = 0;
            command(line);
            len = 0;
            continue;
        }
        if (c == 8) {
            if (len) {
                --len;
                terminal_backspace();
            }
            continue;
        }
        if (c >= 32 && c < 127 && len < sizeof(line) - 1) {
            line[len++] = (char)c;
            terminal_putc((char)c);
        }
    }
}

void kmain(void) {
    if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision))
        halt_forever();

    if (framebuffer_request.response &&
        framebuffer_request.response->framebuffer_count) {
        terminal_init_fb(framebuffer_request.response->framebuffers[0]);
    } else {
        terminal_init();
    }

    terminal_clear();
    boot();
    keyboard_init();
    input_loop();
}
