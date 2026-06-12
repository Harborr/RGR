#include "key_generator.h"
#include <random>
#include <chrono>

std::vector<uint8_t> generate_key(size_t key_size) {
    if (key_size == 0) {
        return {};
    }
    
    std::vector<uint8_t> key(key_size);
    
    // Используем random_device для энтропии
    std::random_device rd;
    
    // Если random_device работает, используем его
    std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    std::mt19937_64 gen(seq);
    
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    
    for (size_t i = 0; i < key_size; i++) {
        key[i] = dist(gen);
    }
    
    return key;
}
