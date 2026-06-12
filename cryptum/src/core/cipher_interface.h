#ifndef CORE_CIPHER_INTERFACE_H
#define CORE_CIPHER_INTERFACE_H

#include <stddef.h>
#include <stdint.h>

struct ConstBuffer {
    const uint8_t* data;
    size_t size;
};

struct MutBuffer {
    uint8_t* data;
    size_t size;
};

struct AlgorithmInfo {
    const char* algorithm_name;
    size_t key_size;   // 0 = переменный
    bool uses_iv;
    size_t iv_size;
};

#endif