#include <stdint.h>

#define CORESIGHT_LAR_OFFSET 0xFB0
#define CORESIGHT_UNLOCK_KEY 0xC5ACCE55

/* MMIO-Hilfsfunktionen für sichere Schreib- und Lesezugriffe */
static inline void mmio_write32(volatile uint32_t *addr, uint32_t val) {
    *(volatile uint32_t *)addr = val;
}

static inline uint32_t mmio_read32(volatile uint32_t *addr) {
    return *(volatile uint32_t *)addr;
}

/**
 * Einstiegspunkt des Payloads
 */
int payload_entry(uint64_t kernel_base) {
    if (kernel_base == 0) {
        return -1;
    }

    /* 
     * Beispiel für das Entsperren des Coresight-Registers
     * Aktiviere und passe die Adresse an:
     */
    volatile uint64_t lar_addr = kernel_base + CORESIGHT_LAR_OFFSET;
    mmio_write32((volatile uint32_t *)lar_addr, CORESIGHT_UNLOCK_KEY);
    
    __asm__ volatile("dsb sy" ::: "memory");
    __asm__ volatile("isb" ::: "memory");

    // Rückgabe 0 signalisiert einen erfolgreichen Durchlauf
    return 0;
}

