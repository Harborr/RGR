#include "args_parser.h"
#include "../core/library_loader.h"
#include <iostream>
#include <cstring>
#include <algorithm>

void print_help(const std::string& program_name) {
    std::cout << "Использование: " << program_name << " [ОПЦИИ]\n\n"
              << "Консольное приложение для шифрования и расшифрования данных.\n\n"
              << "Обязательные аргументы:\n"
              << "  -a, --algorithm ALG    Выбор алгоритма шифрования\n"
              << "  -m, --mode MODE        Режим работы: encrypt, decrypt, generate-key\n\n"
              << "Управление ключом:\n"
              << "  -k, --key FILE         Чтение ключа из файла\n"
              << "  --generate-key         Сгенерировать новый ключ\n"
              << "  --save-key FILE        Сохранить ключ в файл\n"
              << "  --write-key            Вывести ключ в stdout\n\n"
              << "Управление данными:\n"
              << "  -i, --input FILE       Чтение входных данных из файла\n"
              << "  -o, --output FILE      Запись результата в файл\n\n"
              << "Справка:\n"
              << "  -h, --help             Показать эту справку\n\n"
              << "Поддерживаемые алгоритмы:\n";
    
    print_algorithms();
    
    std::cout << "\nПримеры:\n"
              << "  " << program_name << " --help\n"
              << "  " << program_name << " -a vigenere -m encrypt -k key.bin -i plain.txt -o cipher.bin\n"
              << "  " << program_name << " -a vigenere -m generate-key --save-key key.bin\n"
              << "  cat data.bin | " << program_name << " -a vigenere -m encrypt -k key.bin | base64\n";
}

void print_algorithms() {
    auto algorithms = get_available_algorithms();
    for (const auto& algo : algorithms) {
        std::cout << "  - " << algo << "\n";
    }
}

parsed_args parse_arguments(int argc, char* argv[]) {
    parsed_args args;
    
    if (argc < 2) {
        args.mode = program_mode::help;
        args.is_valid = true;
        return args;
    }
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        // Помощь
        if (arg == "-h" || arg == "--help") {
            args.mode = program_mode::help;
            args.is_valid = true;
            return args;
        }
        
        // Алгоритм
        else if (arg == "-a" || arg == "--algorithm") {
            if (i + 1 < argc) {
                args.algorithm = argv[++i];
            } else {
                args.error_message = "Не указано название алгоритма после " + arg;
                return args;
            }
        }
        
        // Режим
        else if (arg == "-m" || arg == "--mode") {
            if (i + 1 < argc) {
                std::string mode_str = argv[++i];
                if (mode_str == "encrypt") {
                    args.mode = program_mode::encrypt;
                } else if (mode_str == "decrypt") {
                    args.mode = program_mode::decrypt;
                } else if (mode_str == "generate-key") {
                    args.mode = program_mode::generate_key;
                } else {
                    args.error_message = "Неизвестный режим: " + mode_str + ". Используйте encrypt, decrypt или generate-key";
                    return args;
                }
            } else {
                args.error_message = "Не указан режим после " + arg;
                return args;
            }
        }
        
        // Ключ из файла
        else if (arg == "-k" || arg == "--key") {
            if (i + 1 < argc) {
                args.key_file = argv[++i];
                args.key_source = data_source::file;
            } else {
                args.error_message = "Не указан файл ключа после " + arg;
                return args;
            }
        }
        
        // Генерация ключа
        else if (arg == "--generate-key") {
            args.generate_key = true;
        }
        
        // Сохранение ключа
        else if (arg == "--save-key") {
            if (i + 1 < argc) {
                args.save_key = true;
                args.save_key_file = argv[++i];
            } else {
                args.error_message = "Не указан файл для сохранения ключа после --save-key";
                return args;
            }
        }
        
        // Вывод ключа в stdout
        else if (arg == "--write-key") {
            args.save_key = true;
            args.save_key_file = "-";  // Специальное имя для stdout
        }
        
        // Входной файл
        else if (arg == "-i" || arg == "--input") {
            if (i + 1 < argc) {
                args.input_file = argv[++i];
                args.input_source = data_source::file;
            } else {
                args.error_message = "Не указан входной файл после " + arg;
                return args;
            }
        }
        
        // Выходной файл
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                args.output_file = argv[++i];
                args.output_source = data_source::file;
            } else {
                args.error_message = "Не указан выходной файл после " + arg;
                return args;
            }
        }
        
        // Неизвестный аргумент
        else {
            args.error_message = "Неизвестный аргумент: " + arg + ". Используйте --help для справки";
            return args;
        }
    }
    
    // Валидация
    if (args.mode == program_mode::help) {
        args.is_valid = true;
        return args;
    }
    
    if (args.algorithm.empty()) {
        args.error_message = "Не указан алгоритм шифрования. Используйте -a или --algorithm";
        return args;
    }
    
    // Проверяем, что алгоритм поддерживается
    auto available = get_available_algorithms();
    if (std::find(available.begin(), available.end(), args.algorithm) == available.end()) {
        args.error_message = "Алгоритм '" + args.algorithm + "' не поддерживается";
        return args;
    }
    
    if (args.mode == program_mode::generate_key) {
        // Для генерации ключа нужен только алгоритм
        args.is_valid = true;
        return args;
    }
    
    // Для encrypt/decrypt нужен ключ или генерация ключа
    if (!args.generate_key && args.key_file.empty()) {
        args.error_message = "Не указан ключ. Используйте -k или --generate-key";
        return args;
    }
    
    // Нельзя одновременно указать ключ и сгенерировать
    if (args.generate_key && !args.key_file.empty()) {
        args.error_message = "Нельзя одновременно указать --generate-key и -k";
        return args;
    }
    
    args.is_valid = true;
    return args;
}
