#ifndef LIBRARY_LOADER_H
#define LIBRARY_LOADER_H

#include "cipher_interface.h"
#include <string>

struct CipherPlugin {
    void* handle;
    const AlgorithmInfo* info;
    size_t(*get_output_size)(size_t, int);
    int (*encrypt)(ConstBuffer, ConstBuffer, MutBuffer*);
    int (*decrypt)(ConstBuffer, ConstBuffer, MutBuffer*);
    int (*encrypt_with_iv)(ConstBuffer, ConstBuffer, ConstBuffer, MutBuffer*);
    void (*free_buffer)(uint8_t*);
};

CipherPlugin load_plugin(const std::string& algorithm);
void unload_plugin(CipherPlugin& plugin);

#endif