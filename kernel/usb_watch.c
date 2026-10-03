#include "usb_watch.h"
#include "kernel.h"
#include <stdint.h>

#define PCI_ADDR 0xCF8
#define PCI_DATA 0xCFC

static int xhci_found;
static uint32_t xhci_mmio;
static int had_usb;
static unsigned gone_ticks;

static uint32_t pci_read32(uint8_t bus,uint8_t dev,uint8_t fn,uint8_t reg) {
    uint32_t a=0x80000000u|((uint32_t)bus<<16)|((uint32_t)dev<<11)|((uint32_t)fn<<8)|(reg&0xFC);
    port_byte_out(PCI_ADDR,(uint8_t)(a>>24)); port_byte_out(PCI_ADDR+1,(uint8_t)(a>>16));
    port_byte_out(PCI_ADDR+2,(uint8_t)(a>>8)); port_byte_out(PCI_ADDR+3,(uint8_t)a);
    return (uint32_t)port_byte_in(PCI_DATA)|((uint32_t)port_byte_in(PCI_DATA+1)<<8)|
           ((uint32_t)port_byte_in(PCI_DATA+2)<<16)|((uint32_t)port_byte_in(PCI_DATA+3)<<24);
}

static void find_xhci(void) {
    for (uint16_t bus=0; bus<256; ++bus) for (uint8_t dev=0; dev<32; ++dev) {
        uint32_t id=pci_read32((uint8_t)bus,dev,0,0);
        if(id==0xFFFFFFFFu) continue;
        uint8_t hdr=(uint8_t)(pci_read32((uint8_t)bus,dev,0,0x0C)>>16);
        uint8_t funcs=(hdr&0x80)?8:1;
        for(uint8_t fn=0; fn<funcs; ++fn) {
            if(pci_read32((uint8_t)bus,dev,fn,0)==0xFFFFFFFFu) continue;
            uint32_t cls=pci_read32((uint8_t)bus,dev,fn,0x08);
            if((uint8_t)(cls>>24)==0x0C && (uint8_t)(cls>>16)==0x03 && (uint8_t)(cls>>8)==0x30) {
                uint32_t bar=pci_read32((uint8_t)bus,dev,fn,0x10);
                if(bar!=0xFFFFFFFFu && !(bar&1u)) { xhci_mmio=bar&0xFFFFFFF0u; xhci_found=1; return; }
            }
        }
    }
}

static int usb_port_connected(void) {
    if(!xhci_found) return 0;
    volatile uint8_t *mmio=(volatile uint8_t *)(uintptr_t)xhci_mmio;
    uint8_t caplen=mmio[0];
    uint32_t hcs1=*(volatile uint32_t *)(mmio+4);
    uint8_t max_ports=(uint8_t)(hcs1>>24);
    if(!max_ports || max_ports>64) return 0;
    uint32_t dboff=*(volatile uint32_t *)(mmio+0x14);
    (void)dboff;
    uint32_t op_base=caplen;
    uint32_t cap2=*(volatile uint32_t *)(mmio+0x10);
    op_base += (cap2 & 0xFFFFFFFCu);
    volatile uint32_t *ports=(volatile uint32_t *)(mmio+op_base+0x400);
    for(uint8_t p=0;p<max_ports;++p) if(ports[p*4]&1u) return 1;
    return 0;
}

static void power_off(void) {
    /* UTM/QEMU power-off fallback. Real firmware may ignore this port. */
    port_byte_out(0xB2,0);
    port_byte_out(0x604,0x00);
    port_byte_out(0x604,0x20);
    for(;;) __asm__ volatile("cli; hlt");
}

void usb_watch_init(void) {
    xhci_found=0; xhci_mmio=0; had_usb=0; gone_ticks=0;
    find_xhci();
    if(xhci_found && usb_port_connected()) had_usb=1;
}

void usb_watch_poll(void) {
    if(!xhci_found || !had_usb) return;
    if(usb_port_connected()) { gone_ticks=0; return; }
    if(++gone_ticks>=200000u) power_off();
}
