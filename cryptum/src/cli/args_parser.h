#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <string>
#include <optional>
#include <vector>

// Режим работы программы
enum class program_mode {
    help,           // Вывод справки
    generate_key,   // Генерация ключа
    encrypt,        // Шифрование
    decrypt         // Расшифрование
};

// Источник/назначение данных
enum class data_source {
    stdin_stream,   // Стандартный поток ввода
    stdout_stream,  // Стандартный поток вывода
    file            // Файл
};

// Результат парсинга аргументов
struct parsed_args {
    program_mode mode = program_mode::help;
    std::string algorithm;
    
    // Ключ
    data_source key_source = data_source::stdin_stream;
    std::string key_file;
    bool generate_key = false;
    
    // Входные данные
    data_source input_source = data_source::stdin_stream;
    std::string input_file;
    
    // Выходные данные
    data_source output_source = data_source::stdout_stream;
    std::string output_file;
    bool save_key = false;
    std::string save_key_file;
    
    // Валидация
    bool is_valid = false;
    std::string error_message;
};

// Распарсить аргументы командной строки
parsed_args parse_arguments(int argc, char* argv[]);

// Вывести справку
void print_help(const std::string& program_name);

// Вывести список алгоритмов
void print_algorithms();

#endif
