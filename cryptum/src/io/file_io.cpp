#include "file_io.h"
#include <fstream>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

std::vector<uint8_t> read_binary_file(const std::string& path) {
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    if (!f) return {};
    size_t size = f.tellg();
    f.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(size);
    f.read(reinterpret_cast<char*>(data.data()), size);
    return data;
}

void write_binary_file(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream f(path, std::ios::binary);
    if (!f) return;
    f.write(reinterpret_cast<const char*>(data.data()), data.size());
}

bool generate_random_bytes(uint8_t* buffer, size_t size) {
#ifdef _WIN32
    HCRYPTPROV prov;
    if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
        return false;
    bool ok = CryptGenRandom(prov, size, buffer);
    CryptReleaseContext(prov, 0);
    return ok;
#else
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) return false;
    ssize_t res = read(fd, buffer, size);
    close(fd);
    return res == (ssize_t)size;
#endif
}