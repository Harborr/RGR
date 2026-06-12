#ifndef FILE_IO_H
#define FILE_IO_H

#include <vector>
#include <string>
#include <cstdint>

// Прочитать файл в вектор байт
std::vector<uint8_t> read_file(const std::string& filename);

// Записать вектор байт в файл
void write_file(const std::string& filename, const std::vector<uint8_t>& data);

// Прочитать все данные из stdin
std::vector<uint8_t> read_stdin();

// Записать данные в stdout
void write_stdout(const std::vector<uint8_t>& data);

// Проверить, существует ли файл
bool file_exists(const std::string& filename);

// Получить размер файла
size_t file_size(const std::string& filename);

#endif
