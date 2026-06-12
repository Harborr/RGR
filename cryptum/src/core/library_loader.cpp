#include "library_loader.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#define DLOPEN LoadLibraryA
#define DLSYM GetProcAddress
#define DLCLOSE FreeLibrary
#else
#include <dlfcn.h>
#define DLOPEN dlopen
#define DLSYM dlsym
#define DLCLOSE dlclose
#endif

CipherPlugin load_plugin(const std::string& algorithm) {
    CipherPlugin p = {};
    std::string lib_name;
#ifdef _WIN32
    lib_name = algorithm + ".dll";
#else
    lib_name = "lib" + algorithm + ".so";
#endif
    void* handle = DLOPEN(lib_name.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load " << lib_name << std::endl;
        return p;
    }
    p.handle = handle;
    p.info = (const AlgorithmInfo*)DLSYM(handle, "get_algorithm_info");
    p.get_output_size = (size_t(*)(size_t, int))DLSYM(handle, "get_output_size");
    p.encrypt = (int(*)(ConstBuffer, ConstBuffer, MutBuffer*))DLSYM(handle, "encrypt");
    p.decrypt = (int(*)(ConstBuffer, ConstBuffer, MutBuffer*))DLSYM(handle, "decrypt");
    p.encrypt_with_iv = (int(*)(ConstBuffer, ConstBuffer, ConstBuffer, MutBuffer*))DLSYM(handle, "encrypt_with_iv");
    p.free_buffer = (void(*)(uint8_t*))DLSYM(handle, "free_buffer");
    if (!p.info || !p.get_output_size || !p.encrypt || !p.decrypt) {
        std::cerr << "Missing symbols in " << lib_name << std::endl;
        DLCLOSE(handle);
        return CipherPlugin{};
    }
    return p;
}

void unload_plugin(CipherPlugin& plugin) {
    if (plugin.handle) DLCLOSE(plugin.handle);
    plugin = CipherPlugin{};
}