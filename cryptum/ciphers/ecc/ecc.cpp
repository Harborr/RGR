#include "../cipher_interface.h"
#include <cstdlib>

static const AlgorithmInfo info = { "ECC (bytewise shift)", 0, false, 0 };

extern "C" const AlgorithmInfo* get_algorithm_info() { return &info; }
extern "C" size_t get_output_size(size_t in, int) { return in; }

static void shift_crypt(ConstBuffer key, ConstBuffer input, MutBuffer* output, bool enc) {
    for (size_t i = 0; i < input.size; ++i) {
        int s = key.data[i % key.size];
        if (!enc) s = -s;
        output->data[i] = (uint8_t)(input.data[i] + s);
    }
}

extern "C" int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size == 0) return -1;
    if (output->size < input.size) return -2;
    shift_crypt(key, input, output, true);
    output->size = input.size;
    return 0;
}
extern "C" int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size == 0) return -1;
    if (output->size < input.size) return -2;
    shift_crypt(key, input, output, false);
    output->size = input.size;
    return 0;
}
extern "C" int encrypt_with_iv(ConstBuffer, ConstBuffer, ConstBuffer, MutBuffer*) { return -1; }
extern "C" void free_buffer(uint8_t* b) { free(b); }