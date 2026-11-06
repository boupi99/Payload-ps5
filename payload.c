#include <stdint.h>

int payload_entry(uint64_t k) {
    if (!k) return -1;
    *(volatile uint32_t *)(k + 0xFB0) = 0xC5ACCE55;
    return 0;
}
