#ifndef LIBRARY_LOADER_H
#define LIBRARY_LOADER_H

#include "cipher_interface.h"
#include <string>
#include <functional>

// Структура для хранения загруженной библиотеки
struct loaded_library_t {
    void* handle;                                          // Указатель на dlopen/LoadLibrary
    
    // Указатели на функции
    const algorithm_info_t* (*get_info)(void);
    size_t (*get_out_size)(size_t, int);
    int (*encrypt)(const_buffer_t, const_buffer_t, mut_buffer_t*);
    int (*decrypt)(const_buffer_t, const_buffer_t, mut_buffer_t*);
    void (*free_buf)(mut_buffer_t*);
    
    algorithm_info_t info;                                 // Кэшированная информация
    std::string name;                                      // Имя алгоритма
};

// Загрузить библиотеку по имени алгоритма
loaded_library_t load_library(const std::string& algorithm_name);

// Выгрузить библиотеку
void unload_library(loaded_library_t& lib);

// Получить список доступных алгоритмов (зашит в коде)
std::vector<std::string> get_available_algorithms();

#endif
