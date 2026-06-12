#include "crypto_operation.h"
#include "io/file_io.h"
#include <cstdio>
#include <vector>

void process_crypto(const CipherPlugin& plugin, const std::vector<uint8_t>& key,
    bool encrypt, const std::string& input_file, const std::string& output_file) {
    FILE* in = (input_file.empty()) ? stdin : fopen(input_file.c_str(), "rb");
    FILE* out = (output_file.empty()) ? stdout : fopen(output_file.c_str(), "wb");
    if (!in || !out) {
        std::cerr << "Failed to open files\n";
        if (in && in != stdin) fclose(in);
        if (out && out != stdout) fclose(out);
        return;
    }

    const size_t CHUNK = 64 * 1024;
    std::vector<uint8_t> in_buf(CHUNK);
    std::vector<uint8_t> out_buf;
    ConstBuffer key_buf{ key.data(), key.size() };

    while (!feof(in)) {
        size_t bytes = fread(in_buf.data(), 1, CHUNK, in);
        if (bytes == 0) break;
        size_t out_size = plugin.get_output_size(bytes, encrypt ? 0 : 1);
        out_buf.resize(out_size);
        MutBuffer out_mut{ out_buf.data(), out_buf.size() };
        int ret;
        if (encrypt)
            ret = plugin.encrypt(key_buf, { in_buf.data(), bytes }, &out_mut);
        else
            ret = plugin.decrypt(key_buf, { in_buf.data(), bytes }, &out_mut);
        if (ret != 0) {
            std::cerr << "Crypto error " << ret << std::endl;
            break;
        }
        fwrite(out_buf.data(), 1, out_mut.size, out);
    }

    if (in != stdin) fclose(in);
    if (out != stdout) fclose(out);
}