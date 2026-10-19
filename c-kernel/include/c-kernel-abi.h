#ifndef CUSTOMOS_C_KERNEL_ABI_H
#define CUSTOMOS_C_KERNEL_ABI_H

/*
 * C kernel <-> XC platform ABI v0.
 * This header defines the boundary; it is not yet wired into kernel startup.
 * Keep this surface small. Device drivers and system services belong in XC.
 */
#include <stddef.h>
#include <stdint.h>

#define CUSTOMOS_KERNEL_ABI_MAJOR 0u
#define CUSTOMOS_KERNEL_ABI_MINOR 1u
#define CUSTOMOS_ABI_STRUCT_VERSION 1u

typedef enum c_kernel_status {
    C_KERNEL_OK = 0,
    C_KERNEL_INVALID_ARGUMENT = 1,
    C_KERNEL_UNSUPPORTED_VERSION = 2,
    C_KERNEL_PERMISSION_DENIED = 3,
    C_KERNEL_RESOURCE_UNAVAILABLE = 4,
    C_KERNEL_OUT_OF_MEMORY = 5,
    C_KERNEL_IO_ERROR = 6
} c_kernel_status_t;

typedef struct c_kernel_abi_version {
    uint16_t major;
    uint16_t minor;
    uint32_t struct_size;
} c_kernel_abi_version_t;

typedef enum c_kernel_memory_kind {
    C_KERNEL_MEMORY_RESERVED = 0,
    C_KERNEL_MEMORY_AVAILABLE = 1,
    C_KERNEL_MEMORY_FIRMWARE_RECLAIMABLE = 2,
    C_KERNEL_MEMORY_ACPI_RECLAIMABLE = 3,
    C_KERNEL_MEMORY_MMIO = 4
} c_kernel_memory_kind_t;

typedef struct c_kernel_memory_region {
    uint32_t struct_size;
    uint32_t kind;
    uint64_t physical_base;
    uint64_t length;
} c_kernel_memory_region_t;

typedef struct c_kernel_framebuffer {
    uint32_t struct_size;
    uint32_t format; /* 0 = unsupported, 1 = RGBX8, 2 = BGRX8 */
    uint64_t physical_base;
    uint64_t byte_length;
    uint32_t width;
    uint32_t height;
    uint32_t pixels_per_scanline;
    uint32_t reserved;
} c_kernel_framebuffer_t;

typedef struct c_kernel_log_record {
    uint32_t struct_size;
    uint32_t severity; /* 0 trace, 1 info, 2 warning, 3 error */
    const char *message;
    size_t message_length;
} c_kernel_log_record_t;

/*
 * ABI rules:
 * - Callers pass struct_size and zero reserved fields.
 * - Physical addresses are descriptive values, never dereferenceable by XC
 *   without an explicit resource capability supplied by the kernel.
 * - All lengths are byte lengths and must be checked for overflow.
 * - New fields are appended; incompatible changes increment the major version.
 * - No ABI function pointers are published until calling convention, ownership,
 *   and privilege rules are specified and tested.
 */

#endif /* CUSTOMOS_C_KERNEL_ABI_H */
