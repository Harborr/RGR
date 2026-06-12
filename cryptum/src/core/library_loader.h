#ifndef LIBRARY_LOADER_H
#define LIBRARY_LOADER_H

#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>

// Структуры из cipher_interface.h (дублируем, чтобы не зависеть от путей)
extern "C" {

struct const_buffer_t {
    const uint8_t* data;
    size_t size;
};

struct mut_buffer_t {
    uint8_t* data;
    size_t size;
};

struct algorithm_info_t {
    const char* algorithm_name;
    size_t key_size;
    size_t iv_size;
    size_t block_size;
};

}

// Структура для хранения загруженной библиотеки
struct loaded_library_t {
    void* handle;
    
    const algorithm_info_t* (*get_info)(void);
    size_t (*get_out_size)(size_t, int);
    int (*encrypt)(const_buffer_t, const_buffer_t, mut_buffer_t*);
    int (*decrypt)(const_buffer_t, const_buffer_t, mut_buffer_t*);
    void (*free_buf)(mut_buffer_t*);
    
    algorithm_info_t info;
    std::string name;
};

// Загрузить библиотеку по имени алгоритма
loaded_library_t load_library(const std::string& algorithm_name);

// Выгрузить библиотеку
void unload_library(loaded_library_t& lib);

// Получить список доступных алгоритмов
std::vector<std::string> get_available_algorithms();

#endif
