#ifndef KEY_GENERATOR_H
#define KEY_GENERATOR_H

#include <vector>
#include <cstdint>
#include <cstddef>

// Сгенерировать криптостойкий ключ заданной длины
std::vector<uint8_t> generate_key(size_t key_size);

#endif
