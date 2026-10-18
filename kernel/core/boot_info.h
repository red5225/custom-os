#include <stddef.h>
#include <stdint.h>

int boot_info_valid(uint64_t magic, uint64_t expected_magic, uint32_t version, uint32_t max_version, uint64_t size, uint64_t min_size);
