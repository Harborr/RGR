#ifndef CRYPTO_OPERATION_H
#define CRYPTO_OPERATION_H

#include "library_loader.h"
#include <vector>
#include <string>

void process_crypto(const CipherPlugin& plugin, const std::vector<uint8_t>& key,
    bool encrypt, const std::string& input_file, const std::string& output_file);

#endif