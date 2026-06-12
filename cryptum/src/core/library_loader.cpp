#include "library_loader.h"
#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
    #define DL_OPEN(name) LoadLibraryA(name)
    #define DL_SYM(handle, name) GetProcAddress((HMODULE)handle, name)
    #define DL_CLOSE(handle) FreeLibrary((HMODULE)handle)
    #define DL_EXT ".dll"
#else
    #include <dlfcn.h>
    #define DL_OPEN(name) dlopen(name, RTLD_NOW)
    #define DL_SYM(handle, name) dlsym(handle, name)
    #define DL_CLOSE(handle) dlclose(handle)
    #define DL_EXT ".so"
#endif

std::vector<std::string> get_available_algorithms() {
    return {
        "vigenere",
        "hill",
        "xor_cipher",
        "transposition",
        "ecc",
        "rabin"
    };
}

loaded_library_t load_library(const std::string& algorithm_name) {
    loaded_library_t lib;
    lib.name = algorithm_name;
    
    std::string lib_filename;
#ifdef _WIN32
    lib_filename = algorithm_name + DL_EXT;
#else
    lib_filename = "lib" + algorithm_name + DL_EXT;
#endif
    
    lib.handle = DL_OPEN(lib_filename.c_str());
    if (!lib.handle) {
        throw std::runtime_error("Не удалось загрузить библиотеку: " + lib_filename);
    }
    
    lib.get_info = reinterpret_cast<const algorithm_info_t*(*)()>(
        DL_SYM(lib.handle, "get_algorithm_info"));
    lib.get_out_size = reinterpret_cast<size_t(*)(size_t, int)>(
        DL_SYM(lib.handle, "get_output_size"));
    lib.encrypt = reinterpret_cast<int(*)(const_buffer_t, const_buffer_t, mut_buffer_t*)>(
        DL_SYM(lib.handle, "encrypt_data"));
    lib.decrypt = reinterpret_cast<int(*)(const_buffer_t, const_buffer_t, mut_buffer_t*)>(
        DL_SYM(lib.handle, "decrypt_data"));
    lib.free_buf = reinterpret_cast<void(*)(mut_buffer_t*)>(
        DL_SYM(lib.handle, "free_buffer"));
    
    if (!lib.get_info || !lib.get_out_size || !lib.encrypt || !lib.decrypt) {
        DL_CLOSE(lib.handle);
        throw std::runtime_error("Библиотека не экспортирует все необходимые функции");
    }
    
    const algorithm_info_t* info = lib.get_info();
    if (info) {
        lib.info = *info;
    }
    
    return lib;
}

void unload_library(loaded_library_t& lib) {
    if (lib.handle) {
        DL_CLOSE(lib.handle);
        lib.handle = nullptr;
    }
}
