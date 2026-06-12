#include "../cipher_interface.h"
#include <cstdlib>

static const AlgorithmInfo info = { "Rabin (affine, mod 256)", 2, false, 0 };

static int mod(int a, int m) { int r = a % m; return r < 0 ? r + m : r; }
static int invmod(int a, int m) {
    a = mod(a, m);
    for (int x = 1; x < m; ++x) if ((a * x) % m == 1) return x;
    return 1;
}

extern "C" const AlgorithmInfo* get_algorithm_info() { return &info; }
extern "C" size_t get_output_size(size_t in, int) { return in; }

extern "C" int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size != 2) return -1;
    if (output->size < input.size) return -2;
    int a = key.data[0], b = key.data[1];
    if ((a & 1) == 0) a = (a + 1) & 0xFF;
    for (size_t i = 0; i < input.size; ++i)
        output->data[i] = (uint8_t)mod(a * input.data[i] + b, 256);
    output->size = input.size;
    return 0;
}
extern "C" int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size != 2) return -1;
    if (output->size < input.size) return -2;
    int a = key.data[0], b = key.data[1];
    if ((a & 1) == 0) a = (a + 1) & 0xFF;
    int a_inv = invmod(a, 256);
    for (size_t i = 0; i < input.size; ++i)
        output->data[i] = (uint8_t)mod(a_inv * (input.data[i] - b), 256);
    output->size = input.size;
    return 0;
}
extern "C" int encrypt_with_iv(ConstBuffer, ConstBuffer, ConstBuffer, MutBuffer*) { return -1; }
extern "C" void free_buffer(uint8_t* b) { free(b); }