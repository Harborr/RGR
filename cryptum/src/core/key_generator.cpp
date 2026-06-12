#include "key_generator.h"
#include "library_loader.h"
#include "io/secure_memory.h"
#include "io/file_io.h"
#include <iostream>
#include <vector>

void generate_and_save_key(const std::string& algorithm, const std::string& save_file, bool write_stdout) {
    CipherPlugin plugin = load_plugin(algorithm);
    if (!plugin.handle) {
        std::cerr << "Cannot load algorithm " << algorithm << std::endl;
        return;
    }
    if (plugin.info->key_size == 0) {
        std::cerr << "Algorithm " << algorithm << " does not have fixed key size. Use --key file.\n";
        return;
    }
    std::vector<uint8_t> key(plugin.info->key_size);
    if (!generate_random_bytes(key.data(), key.size())) {
        std::cerr << "Random generation failed\n";
        return;
    }
    if (!save_file.empty())
        write_binary_file(save_file, key);
    if (write_stdout)
        std::cout.write(reinterpret_cast<char*>(key.data()), key.size());
    secure_zero(key.data(), key.size());
}

std::vector<uint8_t> load_key(const std::string& key_file, bool generate, const std::string& algorithm, const std::string& save_file) {
    if (generate) {
        CipherPlugin plugin = load_plugin(algorithm);
        if (!plugin.handle) return {};
        if (plugin.info->key_size == 0) {
            std::cerr << "Cannot generate key for variable-length algorithm\n";
            return {};
        }
        std::vector<uint8_t> key(plugin.info->key_size);
        if (!generate_random_bytes(key.data(), key.size())) {
            std::cerr << "Key generation failed\n";
            return {};
        }
        if (!save_file.empty()) write_binary_file(save_file, key);
        return key;
    }
    else if (!key_file.empty()) {
        return read_binary_file(key_file);
    }
    std::cerr << "No key provided. Use --key or --generate-key\n";
    return {};
}