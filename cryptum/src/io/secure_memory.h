#ifndef SECURE_MEMORY_H
#define SECURE_MEMORY_H

#include <vector>
#include <cstdint>
#include <string>

// Безопасно очистить вектор (с перезаписью)
void secure_clear(std::vector<uint8_t>& data);

// Безопасно очистить строку
void secure_clear(std::string& str);

#endif
