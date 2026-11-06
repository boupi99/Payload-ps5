#include <stddef.h>
#include <stdint.h>

#define CORESIGHT_LAR_OFFSET  0xFB0
#define CORESIGHT_LSR_OFFSET  0xFB4
#define CORESIGHT_UNLOCK_KEY  0xC5ACCE55
#define CORESIGHT_LOCK_KEY    0x00000000
#define TARGET_REG_OFFSET     0x62000

static inline void mmio_write32(volatile uint64_t addr, uint32_t val) {
    *(volatile uint32_t *)addr = val;
}

static inline uint32_t mmio_read32(volatile uint64_t addr) {
    return *(volatile uint32_t *)addr;
}

int payload_entry(uint64_t kernel_base) {
    volatile uint64_t base_addr = kernel_base; 

    if (!base_addr || base_addr < 0x8000000000000000ULL) {
        return -1;
    }

    mmio_write32(base_addr + CORESIGHT_LAR_OFFSET, CORESIGHT_UNLOCK_KEY);

    uint32_t lsr_val = mmio_read32(base_addr + CORESIGHT_LSR_OFFSET);
    if (lsr_val & 0x2) {
        return -2;
    }

    mmio_write32(base_addr + TARGET_REG_OFFSET, 0x12345678);

    if (mmio_read32(base_addr + TARGET_REG_OFFSET) != 0x12345678) {
        return -3;
    }

    mmio_write32(base_addr + CORESIGHT_LAR_OFFSET, CORESIGHT_LOCK_KEY);

    return 0;
}
