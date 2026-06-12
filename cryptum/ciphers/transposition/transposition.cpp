#include "../cipher_interface.h"
#include <vector>
#include <algorithm>
#include <cstdlib>

static const AlgorithmInfo info = { "Transposition", 0, false, 0 };

static std::vector<size_t> get_order(ConstBuffer key) {
    std::vector<size_t> idx(key.size);
    for (size_t i = 0; i < key.size; ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(), [&](size_t a, size_t b) {return key.data[a] < key.data[b]; });
    return idx;
}

extern "C" const AlgorithmInfo* get_algorithm_info() { return &info; }
extern "C" size_t get_output_size(size_t in, int) { return in; }

extern "C" int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size < 2) return -1;
    if (output->size < input.size) return -2;
    size_t cols = key.size;
    size_t rows = (input.size + cols - 1) / cols;
    size_t pad = rows * cols - input.size;
    std::vector<uint8_t> padded(input.data, input.data + input.size);
    padded.insert(padded.end(), pad, 0xFF);
    auto order = get_order(key);
    size_t pos = 0;
    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
            output->data[pos++] = padded[r * cols + order[c]];
    output->size = input.size; // без паддинга
    return 0;
}

extern "C" int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size < 2) return -1;
    if (output->size < input.size) return -2;
    size_t cols = key.size;
    if (input.size % cols != 0) return -3;
    size_t rows = input.size / cols;
    auto order = get_order(key);
    std::vector<uint8_t> plain(rows * cols);
    size_t pos = 0;
    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
            plain[r * cols + order[c]] = input.data[pos++];
    size_t last = plain.size();
    while (last > 0 && plain[last - 1] == 0xFF) --last;
    if (last > output->size) return -4;
    for (size_t i = 0; i < last; ++i) output->data[i] = plain[i];
    output->size = last;
    return 0;
}

extern "C" int encrypt_with_iv(ConstBuffer, ConstBuffer, ConstBuffer, MutBuffer*) { return -1; }
extern "C" void free_buffer(uint8_t* b) { free(b); }