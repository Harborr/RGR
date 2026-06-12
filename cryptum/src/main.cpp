#include "cli/args_parser.h"
#include "core/library_loader.h"
#include "core/key_generator.h"
#include "io/file_io.h"
#include "io/secure_memory.h"
#include <iostream>
#include <vector>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
#endif

void setup_console() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
}

int do_crypto_operation(const parsed_args& args) {
    loaded_library_t lib;
    try {
        lib = load_library(args.algorithm);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки алгоритма: " << e.what() << std::endl;
        return 1;
    }
    
    std::vector<uint8_t> key_data;
    
    if (args.generate_key) {
        key_data = generate_key(lib.info.key_size);
        std::cerr << "Сгенерирован ключ длиной " << key_data.size() << " байт" << std::endl;
    } else if (!args.key_file.empty()) {
        if (args.key_source == data_source::file) {
            key_data = read_file(args.key_file);
        }
    } else {
        key_data = read_stdin();
    }
    
    if (args.save_key) {
        if (args.save_key_file == "-") {
            write_stdout(key_data);
        } else {
            write_file(args.save_key_file, key_data);
        }
    }
    
    std::vector<uint8_t> input_data;
    if (args.input_source == data_source::file) {
        input_data = read_file(args.input_file);
    } else {
        input_data = read_stdin();
    }
    
    int op_type = (args.mode == program_mode::encrypt) ? 0 : 1;
    size_t output_size = lib.get_out_size(input_data.size(), op_type);
    
    std::vector<uint8_t> output_data(output_size);
    
    const_buffer_t key_buf = { key_data.data(), key_data.size() };
    const_buffer_t input_buf = { input_data.data(), input_data.size() };
    mut_buffer_t output_buf = { output_data.data(), output_data.size() };
    
    int result;
    if (args.mode == program_mode::encrypt) {
        result = lib.encrypt(key_buf, input_buf, &output_buf);
    } else {
        result = lib.decrypt(key_buf, input_buf, &output_buf);
    }
    
    if (result != 0) {
        std::cerr << "Ошибка криптографической операции: код " << result << std::endl;
        secure_clear(key_data);
        unload_library(lib);
        return 1;
    }
    
    output_data.resize(output_buf.size);
    
    if (args.output_source == data_source::file) {
        write_file(args.output_file, output_data);
    } else {
        write_stdout(output_data);
    }
    
    secure_clear(key_data);
    secure_clear(input_data);
    
    unload_library(lib);
    return 0;
}

int main(int argc, char* argv[]) {
    setup_console();
    
    parsed_args args = parse_arguments(argc, argv);
    
    if (!args.error_message.empty()) {
        std::cerr << "Ошибка: " << args.error_message << std::endl;
        std::cerr << "Используйте --help для справки" << std::endl;
        return 1;
    }
    
    if (args.mode == program_mode::help) {
        print_help(argv[0]);
        return 0;
    }
    
    if (args.mode == program_mode::generate_key) {
        std::vector<uint8_t> key_data;
        
        try {
            auto lib = load_library(args.algorithm);
            key_data = generate_key(lib.info.key_size);
            unload_library(lib);
        } catch (...) {
            key_data = generate_key(32);
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
    
    return do_crypto_operation(args);
}
