#include <stdint.h>
#include "kernel.h"
#include "screen.h"
#include "idt.h"
#include "serial.h"
#include "keyboard.h"

#define IDT_ENTRIES 256

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[IDT_ENTRIES];

extern void isr_stub_irq0(void);
extern void isr_stub_irq1(void);
extern void isr_stub_irq12(void);
extern void isr0(void); extern void isr1(void); extern void isr2(void); extern void isr3(void);
extern void isr4(void); extern void isr5(void); extern void isr6(void); extern void isr7(void);
extern void isr8(void); extern void isr9(void); extern void isr10(void); extern void isr11(void);
extern void isr12(void); extern void isr13(void); extern void isr14(void); extern void isr15(void);
extern void isr16(void); extern void isr17(void); extern void isr18(void); extern void isr19(void);
extern void isr20(void); extern void isr21(void); extern void isr22(void); extern void isr23(void);
extern void isr24(void); extern void isr25(void); extern void isr26(void); extern void isr27(void);
extern void isr28(void); extern void isr29(void); extern void isr30(void); extern void isr31(void);
// Optional: future exception stubs could be added here for faults

static inline void lidt(void* base, uint16_t size) {
    struct idt_ptr p; p.base = (uint32_t)base; p.limit = size - 1;
    __asm__ __volatile__("lidt (%0)" :: "r"(&p));
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags; // 0x8E: present, ring0, 32-bit interrupt gate
}

// Minimal regs view matching interrupt stubs stack layout
typedef struct regs_t {
    uint32_t ds, es, fs, gs;         // pushed in stub (ds is top)
    uint32_t edi, esi, ebp, esp;     // pusha
    uint32_t ebx, edx, ecx, eax;     // pusha
    uint32_t int_no, err_code;       // our pushed + CPU/error
    uint32_t eip, cs, eflags;        // CPU pushed
    uint32_t useresp, ss;            // if ring change, else garbage
} regs_t;

static const char* exc_names[32] = {
    "Divide-by-zero",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "BOUND range",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment",
    "Invalid TSS",
    "Segment not present",
    "Stack fault",
    "General protection",
    "Page fault",
    "Reserved",
    "x87 FP exception",
    "Alignment check",
    "Machine check",
    "SIMD FP exception",
    "Virtualization",
    "Control protection",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

static void hex32(uint32_t v, char out[9]) {
    static const char* d = "0123456789ABCDEF";
    for (int i = 7; i >= 0; --i) { out[i] = d[v & 0xF]; v >>= 4; }
    out[8] = '\0';
}

// Panic renderer for CPU exceptions with details
void fault_isr_handler(regs_t* r) {
    serial_write("[panic] exception "); serial_write_dec(r->int_no); serial_write(" err="); serial_write_hex(r->err_code); serial_write("\r\n");
    uint32_t cr2 = 0; if (r->int_no == 14) { __asm__ __volatile__("mov %%cr2, %0" : "=r"(cr2)); }

    disable_cursor();
    clear_screen_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
    draw_border(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
    uint8_t fg = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
    uint8_t hl = vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_RED);
    print_centered("KERNEL PANIC", fg, 1);
    const char* name = (r->int_no < 32) ? exc_names[r->int_no] : "Unknown";
    // Line 3: Vector and name
    char line[64]; char a[9], b[9], c[9];
    hex32(r->err_code, a); hex32(r->eip, b); hex32(cr2, c);
    // Show vector and errcode
    print_centered("Exception details:", hl, 3);
    // Name
    // Build "#NN Name"
    line[0] = '#'; line[1] = '0' + ((r->int_no/10)%10); line[2] = '0' + (r->int_no%10); line[3] = ' '; size_t li = 4; const char* p = name; while (*p && li < sizeof(line)-1) line[li++] = *p++;
    line[li] = '\0'; print_centered(line, fg, 5);
    // err/eip
    const char* pfx1 = "ERR="; const char* pfx2 = " EIP="; size_t x = 2; size_t row = 7;
    print_string(pfx1, hl, x, row); print_string(a, fg, x+4, row);
    print_string(pfx2, hl, x+14, row); print_string(b, fg, x+19, row);
    if (r->int_no == 14) { print_string(" CR2=", hl, x+30, row); print_string(c, fg, x+36, row); }
    // Registers
    row += 2;
    char v[9];
    print_string("EAX=", hl, 2, row); hex32(r->eax, v); print_string(v, fg, 6, row);
    print_string("EBX=", hl, 18, row); hex32(r->ebx, v); print_string(v, fg, 22, row);
    print_string("ECX=", hl, 34, row); hex32(r->ecx, v); print_string(v, fg, 38, row);
    print_string("EDX=", hl, 50, row); hex32(r->edx, v); print_string(v, fg, 54, row);
    row++;
    print_string("ESI=", hl, 2, row); hex32(r->esi, v); print_string(v, fg, 6, row);
    print_string("EDI=", hl, 18, row); hex32(r->edi, v); print_string(v, fg, 22, row);
    print_string("EBP=", hl, 34, row); hex32(r->ebp, v); print_string(v, fg, 38, row);
    print_string("ESP=", hl, 50, row); hex32(r->esp, v); print_string(v, fg, 54, row);
    row++;
    print_centered("System halted.", fg, row+2);
    for(;;) { __asm__ __volatile__("cli; hlt"); }
}

static void pic_remap(void) {
    // Remap PIC1 to 0x20..0x27 and PIC2 to 0x28..0x2F
    uint8_t a1 = port_byte_in(0x21);
    uint8_t a2 = port_byte_in(0xA1);

    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20); // master offset
    port_byte_out(0xA1, 0x28); // slave offset
    port_byte_out(0x21, 0x04); // tell master there is a slave at IRQ2
    port_byte_out(0xA1, 0x02); // tell slave its cascade identity
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);

    // Mask all IRQs for now
    port_byte_out(0x21, 0xFF);
    port_byte_out(0xA1, 0xFF);
    // Small delay and unmask only IRQ0 on master
    uint8_t m = port_byte_in(0x21); (void)m;
    port_byte_out(0x21, 0xFE);

    // Restore masks later if needed
    (void)a1; (void)a2;
}

void idt_init(void) {
    // Zero IDT (no-present by default)
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt[i].base_lo = 0; idt[i].base_hi = 0;
        idt[i].sel = 0; idt[i].always0 = 0; idt[i].flags = 0;
    }

    // Exceptions 0-31 -> individual stubs
    uint16_t cs_sel_exc; __asm__ __volatile__("mov %%cs, %0" : "=r"(cs_sel_exc));
    uint32_t exc_addrs[32] = {
        (uint32_t)isr0,(uint32_t)isr1,(uint32_t)isr2,(uint32_t)isr3,
        (uint32_t)isr4,(uint32_t)isr5,(uint32_t)isr6,(uint32_t)isr7,
        (uint32_t)isr8,(uint32_t)isr9,(uint32_t)isr10,(uint32_t)isr11,
        (uint32_t)isr12,(uint32_t)isr13,(uint32_t)isr14,(uint32_t)isr15,
        (uint32_t)isr16,(uint32_t)isr17,(uint32_t)isr18,(uint32_t)isr19,
        (uint32_t)isr20,(uint32_t)isr21,(uint32_t)isr22,(uint32_t)isr23,
        (uint32_t)isr24,(uint32_t)isr25,(uint32_t)isr26,(uint32_t)isr27,
        (uint32_t)isr28,(uint32_t)isr29,(uint32_t)isr30,(uint32_t)isr31
    };
    for (int v = 0; v < 32; v++) idt_set_gate((uint8_t)v, exc_addrs[v], cs_sel_exc, 0x8E);

    // Remap PIC before enabling any IRQs
    pic_remap();

    // Use current CS selector to avoid GPF if GRUB uses a different selector
    uint16_t cs_sel; __asm__ __volatile__("mov %%cs, %0" : "=r"(cs_sel));
    #if DEBUG
    serial_write("[idt] cs="); serial_write_hex(cs_sel); serial_write("\r\n");
    serial_write("[idt] irq0 handler="); serial_write_hex((uint32_t)isr_stub_irq0); serial_write("\r\n");
    #endif
    idt_set_gate(0x20, (uint32_t)isr_stub_irq0, cs_sel, 0x8E);
    idt_set_gate(0x21, (uint32_t)isr_stub_irq1, cs_sel, 0x8E);
    idt_set_gate(0x2C, (uint32_t)isr_stub_irq12, cs_sel, 0x8E); // IRQ12

    // Load IDT
    lidt(idt, sizeof(idt));
    // Dump IDTR
    #if DEBUG
    struct idt_ptr idtr_read; __asm__ __volatile__("sidt %0" : "=m"(idtr_read));
    serial_write("[idt] idtr.base="); serial_write_hex(idtr_read.base); serial_write(" limit="); serial_write_hex(idtr_read.limit); serial_write("\r\n");
    #endif
}

// PIT timer
void pit_init(uint32_t freq) {
    uint32_t divisor = 1193180 / (freq ? freq : 100);
    port_byte_out(0x43, 0x34); // channel 0, lobyte/hibyte, mode 2 (rate generator)
    port_byte_out(0x40, (uint8_t)(divisor & 0xFF));
    port_byte_out(0x40, (uint8_t)((divisor >> 8) & 0xFF));
    // Unmask IRQ0 on PIC1 now that PIT is configured
    uint8_t mask = port_byte_in(0x21);
    mask &= ~0x01; // clear bit0
    port_byte_out(0x21, mask);
}

// Simple time state
static volatile uint32_t ticks = 0;

uint32_t timer_ticks(void) { return ticks; }

static void draw_footer(void) {
    const char spin[4] = {'|','/','-','\\'};
    char buf[16];
    uint32_t t = ticks / 100; // assuming 100 Hz
    uint32_t s = t % 60;
    uint32_t m = (t / 60) % 60;
    uint32_t h = (t / 3600) % 24;

    // Build HH:MM:SS
    buf[0] = '0' + (h/10); buf[1] = '0' + (h%10);
    buf[2] = ':'; buf[3] = '0' + (m/10); buf[4] = '0' + (m%10);
    buf[5] = ':'; buf[6] = '0' + (s/10); buf[7] = '0' + (s%10);
    buf[8] = ' '; buf[9] = spin[(ticks/5) & 3]; buf[10] = '\0';

    uint8_t bar_fg = vga_entry_color(VGA_COLOR_YELLOW, VGA_COLOR_BLUE);
    // right align at bottom-2 row (status bar row)
    size_t row = MAX_ROWS - 2;
    size_t len = 10;
    size_t x = (MAX_COLS > len+2) ? (MAX_COLS - len - 2) : 0;
    print_string(buf, bar_fg, x, row);
}

// C handler called from IRQ0 stub
void timer_isr_handler(void) {
    ticks++;
    if ((ticks % 5) == 0) { // update at 20Hz for visible spinner
        extern int g_fb_mode; if (!g_fb_mode) {
            draw_footer();
        }
        // light serial heartbeat
        // serial_write("[irq0].");
    }
    // Drive keyboard key repeat at 100Hz base
    keyboard_on_timer_tick();
    extern int g_fb_mode; extern int g_gui_enabled; if (g_fb_mode && g_gui_enabled) { extern void gui_tick(void); gui_tick(); }
    // EOI to PIC
    port_byte_out(0x20, 0x20);
}