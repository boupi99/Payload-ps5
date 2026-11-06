#include <stdint.h>

#define LAR_OFF 0xFB0
#define UNLOCK 0xC5ACCE55

int payload_entry(uint64_t kbase) {
    if (kbase == 0) return -1;
    
    volatile uint32_t *lar = 
        (volatile uint32_t *)(kbase + LAR_OFF);
    *lar = UNLOCK;
    
    __asm__ volatile("dsb sy" ::: "memory");
    __asm__ volatile("isb" ::: "memory");
    
    return 0;
}
