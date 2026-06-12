#ifndef CIPHER_INTERFACE_H
#define CIPHER_INTERFACE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// === СТРУКТУРЫ ДЛЯ ПЕРЕДАЧИ ДАННЫХ ===

// Константный буфер (для входных данных и ключа)
typedef struct {
    const uint8_t* data;
    size_t size;
} const_buffer_t;

// Изменяемый буфер (для выходных данных)
typedef struct {
    uint8_t* data;
    size_t size;
} mut_buffer_t;

// Информация об алгоритме
typedef struct {
    const char* algorithm_name;   // Название алгоритма
    size_t key_size;              // Размер ключа в байтах
    size_t iv_size;               // Размер IV (0 если не используется)
    size_t block_size;            // Размер блока (1 для поточных)
} algorithm_info_t;

// === ФУНКЦИИ, КОТОРЫЕ ДОЛЖНА ЭКСПОРТИРОВАТЬ КАЖДАЯ БИБЛИОТЕКА ===

// Получить информацию об алгоритме
const algorithm_info_t* get_algorithm_info(void);

// Вычислить размер выходного буфера
// operation_type: 0 = encrypt, 1 = decrypt
size_t get_output_size(size_t input_size, int operation_type);

// Зашифровать данные
// Возвращает 0 при успехе, код ошибки при неудаче
int encrypt_data(const_buffer_t key, const_buffer_t input, mut_buffer_t* output);

// Расшифровать данные
// Возвращает 0 при успехе, код ошибки при неудаче
int decrypt_data(const_buffer_t key, const_buffer_t input, mut_buffer_t* output);

// Очистить память (если библиотека выделяла)
void free_buffer(mut_buffer_t* buffer);

#ifdef __cplusplus
}
#endif

#endif // CIPHER_INTERFACE_H
