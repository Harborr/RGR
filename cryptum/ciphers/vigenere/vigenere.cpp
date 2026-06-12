#include "../cipher_interface.h"
#include <cstring>
#include <cstdlib>

// Встроенный алфавит (256 байт — все возможные значения)
static const size_t ALPHABET_SIZE = 256;

static algorithm_info_t info = {
    "vigenere",
    32,     // key_size: до 32 байт
    0,      // iv_size: не используется
    1       // block_size: поточный шифр
};

const algorithm_info_t* get_algorithm_info(void) {
    return &info;
}

size_t get_output_size(size_t input_size, int operation_type) {
    (void)operation_type;
    return input_size;  // Поточный шифр не меняет размер
}

int encrypt_data(const_buffer_t key, const_buffer_t input, mut_buffer_t* output) {
    if (!key.data || !input.data || !output || !output->data) return 1;
    if (output->size < input.size) return 2;
    
    for (size_t i = 0; i < input.size; i++) {
        uint8_t k = key.data[i % key.size];
        output->data[i] = (input.data[i] + k) % ALPHABET_SIZE;
    }
    output->size = input.size;
    return 0;
}

int decrypt_data(const_buffer_t key, const_buffer_t input, mut_buffer_t* output) {
    if (!key.data || !input.data || !output || !output->data) return 1;
    if (output->size < input.size) return 2;
    
    for (size_t i = 0; i < input.size; i++) {
        uint8_t k = key.data[i % key.size];
        output->data[i] = (input.data[i] - k + ALPHABET_SIZE) % ALPHABET_SIZE;
    }
    output->size = input.size;
    return 0;
}

void free_buffer(mut_buffer_t* buffer) {
    if (buffer && buffer->data) {
        // Затираем данные перед освобождением
        memset(buffer->data, 0, buffer->size);
        free(buffer->data);
        buffer->data = nullptr;
        buffer->size = 0;
    }
}
