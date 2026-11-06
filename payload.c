#include <stddef.h>
#include <stdint.h>

#define CORESIGHT_LAR_OFFSET 0xFB0
#define CORESIGHT_LSR_OFFSET 0xFB4
#define CORESIGHT_UNLOCK_KEY 0xC5ACCE55
#define TARGET_REG_OFFSET    0x62000

static inline void mmio_write32(volatile uint64_t addr, uint32_t val) {
    *(volatile uint32_t *)addr = val;
}

static inline uint32_t mmio_read32(volatile uint64_t addr) {
    return *(volatile uint32_t *)addr;
}

int payload_entry(uint64_t kernel_base) {
    volatile uint64_t base_addr = kernel_base;

    // 1. CoreSight-Hardware entsperren
    mmio_write32(base_addr + CORESIGHT_LAR_OFFSET, CORESIGHT_UNLOCK_KEY);

    // 2. Status prüfen (Bit 1 muss 0 sein)
    uint32_t lsr = mmio_read32(base_addr + CORESIGHT_LSR_OFFSET);
    if (lsr & 0x2) {
        return -1; // Fehler: Immer noch gesperrt
    }

    // 3. Schreibzugriff auf das Zielregister ausführen
    mmio_write32(base_addr + TARGET_REG_OFFSET, 0x12345678);

    return 0; // Erfolg
}

