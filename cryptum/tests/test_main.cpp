#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>

#ifdef _WIN32
    #include <windows.h>
    #define DL_OPEN(name) LoadLibraryA(name)
    #define DL_SYM(handle, name) GetProcAddress((HMODULE)handle, name)
    #define DL_CLOSE(handle) FreeLibrary((HMODULE)handle)
#else
    #include <dlfcn.h>
    #define DL_OPEN(name) dlopen(name, RTLD_NOW)
    #define DL_SYM(handle, name) dlsym(handle, name)
    #define DL_CLOSE(handle) dlclose(handle)
#endif

#include "../ciphers/cipher_interface.h"

// Функция для тестирования одного алгоритма
bool test_algorithm(const char* lib_name) {
    std::cout << "Тестирование " << lib_name << "... ";
    
    // Загружаем библиотеку
    void* handle = DL_OPEN(lib_name);
    if (!handle) {
        std::cout << "ОШИБКА: не удалось загрузить библиотеку" << std::endl;
        return false;
    }
    
    // Загружаем функции
    auto get_info = (const algorithm_info_t*(*)())DL_SYM(handle, "get_algorithm_info");
    auto get_size = (size_t(*)(size_t, int))DL_SYM(handle, "get_output_size");
    auto encrypt = (int(*)(const_buffer_t, const_buffer_t, mut_buffer_t*))DL_SYM(handle, "encrypt_data");
    auto decrypt = (int(*)(const_buffer_t, const_buffer_t, mut_buffer_t*))DL_SYM(handle, "decrypt_data");
    
    if (!get_info || !get_size || !encrypt || !decrypt) {
        std::cout << "ОШИБКА: не все функции экспортированы" << std::endl;
        DL_CLOSE(handle);
        return false;
    }
    
    auto* info = get_info();
    std::cout << info->algorithm_name << " (ключ: " << info->key_size << " байт)... ";
    
    // Тест 1: пустые данные
    {
        std::vector<uint8_t> key(info->key_size, 0x42);
        std::vector<uint8_t> input;
        size_t out_size = get_size(0, 0);
        std::vector<uint8_t> output(out_size);
        
        const_buffer_t key_buf = { key.data(), key.size() };
        const_buffer_t in_buf = { input.data(), input.size() };
        mut_buffer_t out_buf = { output.data(), output.size() };
        
        int r1 = encrypt(key_buf, in_buf, &out_buf);
        output.resize(out_buf.size);
        
        std::vector<uint8_t> decrypted(output.size());
        const_buffer_t enc_buf = { output.data(), output.size() };
        mut_buffer_t dec_buf = { decrypted.data(), decrypted.size() };
        
        int r2 = decrypt(key_buf, enc_buf, &dec_buf);
        
        if (r1 != 0 || r2 != 0) {
            std::cout << "ОШИБКА в тесте 1" << std::endl;
            DL_CLOSE(handle);
            return false;
        }
    }
    
    // Тест 2: короткие данные
    {
        std::vector<uint8_t> key(info->key_size, 0x13);
        std::vector<uint8_t> input = {0x48, 0x65, 0x6C, 0x6C, 0x6F};  // "Hello"
        
        size_t out_size = get_size(input.size(), 0);
        std::vector<uint8_t> output(out_size);
        
        const_buffer_t key_buf = { key.data(), key.size() };
        const_buffer_t in_buf = { input.data(), input.size() };
        mut_buffer_t out_buf = { output.data(), output.size() };
        
        encrypt(key_buf, in_buf, &out_buf);
        output.resize(out_buf.size);
        
        std::vector<uint8_t> decrypted(out_buf.size);
        const_buffer_t enc_buf = { output.data(), output.size() };
        mut_buffer_t dec_buf = { decrypted.data(), decrypted.size() };
        
        decrypt(key_buf, enc_buf, &dec_buf);
        decrypted.resize(dec_buf.size);
        
        if (input != decrypted) {
            std::cout << "ОШИБКА в тесте 2: данные не совпадают" << std::endl;
            DL_CLOSE(handle);
            return false;
        }
    }
    
    // Тест 3: длинные данные
    {
        std::vector<uint8_t> key(info->key_size);
        for (size_t i = 0; i < key.size(); i++) key[i] = i % 256;
        
        std::vector<uint8_t> input(1024);
        for (size_t i = 0; i < input.size(); i++) input[i] = (i * 7) % 256;
        
        size_t out_size = get_size(input.size(), 0);
        std::vector<uint8_t> output(out_size);
        
        const_buffer_t key_buf = { key.data(), key.size() };
        const_buffer_t in_buf = { input.data(), input.size() };
        mut_buffer_t out_buf = { output.data(), output.size() };
        
        encrypt(key_buf, in_buf, &out_buf);
        output.resize(out_buf.size);
        
        std::vector<uint8_t> decrypted(out_buf.size);
        const_buffer_t enc_buf = { output.data(), output.size() };
        mut_buffer_t dec_buf = { decrypted.data(), decrypted.size() };
        
        decrypt(key_buf, enc_buf, &dec_buf);
        decrypted.resize(dec_buf.size);
        
        if (input != decrypted) {
            std::cout << "ОШИБКА в тесте 3: данные не совпадают" << std::endl;
            DL_CLOSE(handle);
            return false;
        }
    }
    
    std::cout << "OK" << std::endl;
    DL_CLOSE(handle);
    return true;
}

int main() {
    std::cout << "=== Тестирование криптографических алгоритмов ===" << std::endl;
    
    bool all_ok = true;
    
#ifdef _WIN32
    all_ok &= test_algorithm("vigenere.dll");
    all_ok &= test_algorithm("xor_cipher.dll");
    all_ok &= test_algorithm("transposition.dll");
#else
    all_ok &= test_algorithm("./libvigenere.so");
    all_ok &= test_algorithm("./libxor_cipher.so");
    all_ok &= test_algorithm("./libtransposition.so");
#endif
    
    std::cout << std::endl;
    if (all_ok) {
        std::cout << "Все тесты пройдены успешно!" << std::endl;
        return 0;
    } else {
        std::cout << "Некоторые тесты не пройдены!" << std::endl;
        return 1;
    }
}
