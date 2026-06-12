#include "key_generator.h"
#include <random>
#include <algorithm>

std::vector<uint8_t> generate_key(size_t key_size) {
    if (key_size == 0) {
        return {};
    }
    
    std::vector<uint8_t> key(key_size);
    
    // Используем std::random_device для криптостойкой энтропии
    std::random_device rd;
    
    // Если random_device не доступен, используем время как fallback
    std::mt19937_64 gen;
    
    try {
        // Пытаемся использовать аппаратный генератор
        std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
        gen.seed(seq);
    } catch (...) {
        // Fallback на время и адреса памяти
        auto now = std::chrono::high_resolution_clock::now();
        auto seed = now.time_since_epoch().count();
        std::seed_seq seq{static_cast<unsigned int>(seed),
                          static_cast<unsigned int>(seed >> 32),
                          rd()};
        gen.seed(seq);
    }
    
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    
    for (size_t i = 0; i < key_size; i++) {
        key[i] = dist(gen);
    }
    
    return key;
}
