#include <stdint.h>

#define CORESIGHT_LAR_OFFSET 0xFB0
#define CORESIGHT_UNLOCK_KEY 0xC5ACCE55

static inline void mmio_write32(volatile uint32_t *addr, uint32_t val) {
    *(volatile uint32_t *)addr = val;
}

static inline uint32_t mmio_read32(volatile uint32_t *addr) {
    return *(volatile uint32_t *)addr;
}

int payload_entry(uint64_t kernel_base) {
    if (kernel_base == 0) {
        return -1;
    }

    volatile uint64_t lar_addr = kernel_base + CORESIGHT_LAR_OFFSET;
    mmio_write32((volatile uint32_t *)lar_addr, CORESIGHT_UNLOCK_KEY);
    
    __asm__ volatile("dsb sy" ::: "memory");
    __asm__ volatile("isb" ::: "memory");

    return 0;
}