#include "../cipher_interface.h"
#include <cstring>
#include <cstdlib>

static const AlgorithmInfo info = { "Hill (2x2, mod 256)", 4, false, 0 };

static int mod(int a, int m) { int r = a % m; return r < 0 ? r + m : r; }
static int invmod(int a, int m) {
    a = mod(a, m);
    for (int x = 1; x < m; ++x) if ((a * x) % m == 1) return x;
    return -1;
}

static bool invert_matrix(const int m[2][2], int inv[2][2]) {
    int det = mod(m[0][0] * m[1][1] - m[0][1] * m[1][0], 256);
    int idet = invmod(det, 256);
    if (idet == -1) return false;
    inv[0][0] = mod(m[1][1] * idet, 256);
    inv[0][1] = mod(-m[0][1] * idet, 256);
    inv[1][0] = mod(-m[1][0] * idet, 256);
    inv[1][1] = mod(m[0][0] * idet, 256);
    return true;
}

static void mul_matrix(const int m[2][2], const uint8_t* in, uint8_t* out) {
    out[0] = mod(m[0][0] * in[0] + m[0][1] * in[1], 256);
    out[1] = mod(m[1][0] * in[0] + m[1][1] * in[1], 256);
}

extern "C" const AlgorithmInfo* get_algorithm_info() { return &info; }
extern "C" size_t get_output_size(size_t in, int) { return in; }

extern "C" int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size != 4) return -1;
    if (output->size < input.size) return -2;
    int m[2][2];
    for (int i = 0; i < 4; ++i) m[i / 2][i % 2] = key.data[i];
    for (size_t i = 0; i + 1 < input.size; i += 2)
        mul_matrix(m, input.data + i, output->data + i);
    if (input.size % 2) output->data[input.size - 1] = input.data[input.size - 1];
    output->size = input.size;
    return 0;
}

extern "C" int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size != 4) return -1;
    if (output->size < input.size) return -2;
    int m[2][2], inv[2][2];
    for (int i = 0; i < 4; ++i) m[i / 2][i % 2] = key.data[i];
    if (!invert_matrix(m, inv)) return -3;
    for (size_t i = 0; i + 1 < input.size; i += 2)
        mul_matrix(inv, input.data + i, output->data + i);
    if (input.size % 2) output->data[input.size - 1] = input.data[input.size - 1];
    output->size = input.size;
    return 0;
}

extern "C" int encrypt_with_iv(ConstBuffer, ConstBuffer, ConstBuffer, MutBuffer*) { return -1; }
extern "C" void free_buffer(uint8_t* b) { free(b); }