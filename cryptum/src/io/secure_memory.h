#ifndef SECURE_MEMORY_H
#define SECURE_MEMORY_H

#include <cstddef>
#include <cstdint>

void secure_zero(uint8_t* buffer, size_t size);

#endif