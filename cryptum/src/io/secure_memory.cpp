#include "secure_memory.h"
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
#endif

// Функция для очистки памяти, которую компилятор не оптимизирует
static void secure_zero(void* ptr, size_t size) {
    volatile uint8_t* p = static_cast<volatile uint8_t*>(ptr);
    while (size--) {
        *p++ = 0;
    }
}

void secure_clear(std::vector<uint8_t>& data) {
    if (!data.empty()) {
        secure_zero(data.data(), data.size());
        data.clear();
    }
}

void secure_clear(uint8_t* data, size_t size) {
    if (data && size > 0) {
        secure_zero(data, size);
    }
}

void memory_lock(const void* addr, size_t size) {
#ifdef _WIN32
    VirtualLock(const_cast<LPVOID>(addr), size);
#else
    mlock(addr, size);
#endif
}

void memory_unlock(const void* addr, size_t size) {
#ifdef _WIN32
    VirtualUnlock(const_cast<LPVOID>(addr), size);
#else
    munlock(addr, size);
#endif
}
