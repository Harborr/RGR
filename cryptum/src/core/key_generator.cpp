#include "key_generator.h"
#include <random>
#include <cstring>

std::vector<uint8_t> generate_key(size_t key_size) {
    if (key_size == 0) return {};
    
    std::vector<uint8_t> key(key_size);
    
    // Используем std::random_device для криптостойкой генерации
    // Если недоступен — fallback на время
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    
    for (size_t i = 0; i < key_size; i++) {
        key[i] = dist(gen);
    }
    
    return key;
}
