#include "secure_memory.h"
#include <cstring>

// Платформозависимая функция для защиты от оптимизации компилятора
#ifdef _WIN32
    #include <windows.h>
    #define SECURE_ZERO(ptr, size) SecureZeroMemory(ptr, size)
#else
    #include <cstring>
    static void secure_zero(void* ptr, size_t size) {
        volatile uint8_t* p = static_cast<volatile uint8_t*>(ptr);
        for (size_t i = 0; i < size; i++) {
            p[i] = 0;
        }
    }
    #define SECURE_ZERO(ptr, size) secure_zero(ptr, size)
#endif

void secure_clear(std::vector<uint8_t>& data) {
    if (!data.empty()) {
        SECURE_ZERO(data.data(), data.size());
        data.clear();
    }
}

void secure_clear(std::string& str) {
    if (!str.empty()) {
        SECURE_ZERO(&str[0], str.size());
        str.clear();
    }
}
