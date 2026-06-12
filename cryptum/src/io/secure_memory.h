#ifndef SECURE_MEMORY_H
#define SECURE_MEMORY_H

#include <vector>
#include <cstdint>
#include <cstddef>

// Безопасно очистить память (затереть данные)
void secure_clear(std::vector<uint8_t>& data);

// Безопасно очистить C-массив
void secure_clear(uint8_t* data, size_t size);

// Заблокировать страницу памяти от свопинга (Linux/BSD)
void memory_lock(const void* addr, size_t size);

// Разблокировать страницу памяти
void memory_unlock(const void* addr, size_t size);

#endif
