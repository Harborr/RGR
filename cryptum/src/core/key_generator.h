#ifndef KEY_GENERATOR_H
#define KEY_GENERATOR_H

#include <string>
#include <vector>

void generate_and_save_key(const std::string& algorithm, const std::string& save_file, bool write_stdout);
std::vector<uint8_t> load_key(const std::string& key_file, bool generate, const std::string& algorithm, const std::string& save_file);

#endif