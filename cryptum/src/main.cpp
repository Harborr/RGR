#include "cli/args_parser.h"
#include "core/library_loader.h"
#include "core/key_generator.h"
#include "io/file_io.h"
#include <iostream>
#include <vector>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
#endif

// Настройка кодировки UTF-8 для Windows
void setup_console() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
}

// Выполнить операцию шифрования/расшифрования
int do_crypto_operation(const parsed_args& args) {
    // Загружаем библиотеку алгоритма
    loaded_library_t lib;
    try {
        lib = load_library(args.algorithm);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки алгоритма: " << e.what() << std::endl;
        return 1;
    }
    
    // Получаем информацию об алгоритме
    std::cout << "Алгоритм: " << lib.info.algorithm_name << std::endl;
    std::cout << "Размер ключа: " << lib.info.key_size << " байт" << std::endl;
    
    // Получаем или генерируем ключ
    std::vector<uint8_t> key_data;
    
    if (args.generate_key) {
        key_data = generate_key(lib.info.key_size);
        std::cerr << "Сгенерирован ключ длиной " << key_data.size() << " байт" << std::endl;
    } else if (!args.key_file.empty()) {
        if (args.key_source == data_source::file) {
            key_data = read_file(args.key_file);
        }
    } else {
        // Читаем ключ из stdin
        key_data = read_stdin();
    }
    
    // Проверяем размер ключа
    if (key_data.size() != lib.info.key_size) {
        std::cerr << "Предупреждение: размер ключа (" << key_data.size() 
                  << ") отличается от требуемого (" << lib.info.key_size << ")" << std::endl;
    }
    
    // Сохраняем ключ если нужно
    if (args.save_key) {
        if (args.save_key_file == "-") {
            write_stdout(key_data);
        } else {
            write_file(args.save_key_file, key_data);
        }
    }
    
    // Читаем входные данные
    std::vector<uint8_t> input_data;
    if (args.input_source == data_source::file) {
        input_data = read_file(args.input_file);
    } else {
        input_data = read_stdin();
    }
    
    // Вычисляем размер выходных данных
    int op_type = (args.mode == program_mode::encrypt) ? 0 : 1;
    size_t output_size = lib.get_out_size(input_data.size(), op_type);
    
    // Выделяем память под выходные данные
    std::vector<uint8_t> output_data(output_size);
    
    // Подготавливаем структуры
    const_buffer_t key_buf = { key_data.data(), key_data.size() };
    const_buffer_t input_buf = { input_data.data(), input_data.size() };
    mut_buffer_t output_buf = { output_data.data(), output_data.size() };
    
    // Выполняем операцию
    int result;
    if (args.mode == program_mode::encrypt) {
        result = lib.encrypt(key_buf, input_buf, &output_buf);
    } else {
        result = lib.decrypt(key_buf, input_buf, &output_buf);
    }
    
    if (result != 0) {
        std::cerr << "Ошибка криптографической операции: код " << result << std::endl;
        unload_library(lib);
        return 1;
    }
    
    // Обновляем размер (мог измениться)
    output_data.resize(output_buf.size);
    
    // Выводим результат
    if (args.output_source == data_source::file) {
        write_file(args.output_file, output_data);
    } else {
        write_stdout(output_data);
    }
    
    // Очищаем sensitive данные
    secure_clear(key_data);
    secure_clear(input_data);
    
    unload_library(lib);
    return 0;
}

int main(int argc, char* argv[]) {
    setup_console();
    
    // Парсим аргументы
    parsed_args args = parse_arguments(argc, argv);
    
    // Обработка ошибок парсинга
    if (!args.error_message.empty()) {
        std::cerr << "Ошибка: " << args.error_message << std::endl;
        std::cerr << "Используйте --help для справки" << std::endl;
        return 1;
    }
    
    // Режим справки
    if (args.mode == program_mode::help) {
        print_help(argv[0]);
        return 0;
    }
    
    // Режим генерации ключа
    if (args.mode == program_mode::generate_key) {
        // Получаем размер ключа из информации об алгоритме
        // (можно было бы загрузить библиотеку, но для скорости просто используем значения)
        std::vector<uint8_t> key_data;
        
        try {
            auto lib = load_library(args.algorithm);
            key_data = generate_key(lib.info.key_size);
            unload_library(lib);
        } catch (...) {
            key_data = generate_key(32);  // По умолчанию 32 байта
        }
        
        if (args.save_key) {
            if (args.save_key_file == "-") {
                write_stdout(key_data);
            } else {
                write_file(args.save_key_file, key_data);
            }
        } else {
            write_stdout(key_data);
        }
        
        secure_clear(key_data);
        return 0;
    }
    
    // Режим шифрования/расшифрования
    return do_crypto_operation(args);
}
