#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <vector>
#include <cstdint>

std::vector<uint8_t> read_binary_file(const std::string& path);
void write_binary_file(const std::string& path, const std::vector<uint8_t>& data);
bool generate_random_bytes(uint8_t* buffer, size_t size);

#endif