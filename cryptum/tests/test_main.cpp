#include "../ciphers/cipher_interface.h"
#include "../src/io/file_io.h"
#include "../src/io/secure_memory.h"
#include <dlfcn.h>
#include <iostream>
#include <vector>
#include <cassert>

static const char* algs[] = { "vigenere","hill","xor_cipher","transposition","ecc","rabin" };
static const char* libs[] = { "libvigenere.so","libhill.so","libxor_cipher.so","libtransposition.so","libecc.so","librabin.so" };

bool test_one(const char* lib, size_t expected_key_size) {
    void* h = dlopen(lib, RTLD_LAZY);
    if (!h) { std::cerr << "Cannot load " << lib << std::endl; return false; }
    auto get_info = (const AlgorithmInfo * (*)())dlsym(h, "get_algorithm_info");
    auto enc = (int(*)(ConstBuffer, ConstBuffer, MutBuffer*))dlsym(h, "encrypt");
    auto dec = (int(*)(ConstBuffer, ConstBuffer, MutBuffer*))dlsym(h, "decrypt");
    if (!get_info || !enc || !dec) { dlclose(h); return false; }
    const AlgorithmInfo* info = get_info();
    std::vector<uint8_t> key;
    if (info->key_size > 0) key.resize(info->key_size, 0x42);
    else key = { 0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF };

    // Test 1: fixed pattern
    std::vector<uint8_t> plain = { 'H','e','l','l','o',0,0xFF };
    size_t out_sz = info->key_size ? plain.size() : plain.size(); // dummy
    std::vector<uint8_t> cipher(out_sz);
    MutBuffer out{ cipher.data(), cipher.size() };
    assert(enc({ key.data(),key.size() }, { plain.data(),plain.size() }, &out) == 0);
    std::vector<uint8_t> dec_plain(out.size);
    MutBuffer dec_buf{ dec_plain.data(), dec_plain.size() };
    assert(dec({ key.data(),key.size() }, { cipher.data(),out.size }, &dec_buf) == 0);
    assert(dec_buf.size == plain.size());
    assert(memcmp(dec_plain.data(), plain.data(), plain.size()) == 0);

    // Test 2: random 1KB
    std::vector<uint8_t> rnd(1024);
    generate_random_bytes(rnd.data(), rnd.size());
    out_sz = rnd.size();
    cipher.resize(out_sz);
    out = { cipher.data(), cipher.size() };
    assert(enc({ key.data(),key.size() }, { rnd.data(),rnd.size() }, &out) == 0);
    dec_plain.resize(out.size);
    dec_buf = { dec_plain.data(), dec_plain.size() };
    assert(dec({ key.data(),key.size() }, { cipher.data(),out.size }, &dec_buf) == 0);
    assert(dec_buf.size == rnd.size());
    assert(memcmp(dec_plain.data(), rnd.data(), rnd.size()) == 0);

    dlclose(h);
    return true;
}

int main() {
    bool ok = true;
    for (size_t i = 0; i < 6; ++i) {
        std::cout << "Testing " << algs[i] << "... ";
        if (test_one(libs[i], 0)) std::cout << "OK\n";
        else { std::cout << "FAIL\n"; ok = false; }
    }
    return ok ? 0 : 1;
}