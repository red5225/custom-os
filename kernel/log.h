#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include <stddef.h>

void klog_init(void);
void klog_write_char(char c);
void klog_write(const char* s);
void klog_write_hex(uint32_t v);
void klog_write_dec(uint32_t v);

// Query API for viewer
size_t klog_line_count(void);             // number of stored lines (<= capacity)
const char* klog_get_line(size_t index);  // index 0..count-1 (0 oldest)

#endif
