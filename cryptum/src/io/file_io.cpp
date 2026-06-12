#include "file_io.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>
#endif

std::vector<uint8_t> read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + filename);
    }
    
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    file.close();
    
    return data;
}

void write_file(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    file.close();
}

std::vector<uint8_t> read_stdin() {
    std::vector<uint8_t> data;
    uint8_t buffer[4096];
    
    // Переключаем stdin в бинарный режим (Windows)
#ifdef _WIN32
    _setmode(_fileno(stdin), _O_BINARY);
#endif
    
    while (!std::cin.eof()) {
        std::cin.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
        size_t bytes_read = std::cin.gcount();
        if (bytes_read > 0) {
            data.insert(data.end(), buffer, buffer + bytes_read);
        }
    }
    
    return data;
}

void write_stdout(const std::vector<uint8_t>& data) {
    // Переключаем stdout в бинарный режим (Windows)
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif
    
    std::cout.write(reinterpret_cast<const char*>(data.data()), data.size());
    std::cout.flush();
}

bool file_exists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

size_t file_size(const std::string& filename) {
    struct stat buffer;
    if (stat(filename.c_str(), &buffer) == 0) {
        return buffer.st_size;
    }
    return 0;
}
