#include "secure_memory.h"

void secure_zero(uint8_t* buffer, size_t size) {
    volatile uint8_t* p = buffer;
    for (size_t i = 0; i < size; ++i) p[i] = 0;
}