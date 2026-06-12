#include "cli/args_parser.h"
#include "core/crypto_operation.h"
#include "core/library_loader.h"
#include "core/key_generator.h"
#include "io/file_io.h"
#include "io/secure_memory.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    try {
        Args args = parse_args(argc, argv);

        if (args.help) {
            print_help();
            return 0;
        }

        if (args.mode == Mode::GENERATE_KEY) {
            generate_and_save_key(args.algorithm, args.save_key, args.write_key_stdout);
            return 0;
        }

        if (args.mode == Mode::ENCRYPT || args.mode == Mode::DECRYPT) {
            CipherPlugin plugin = load_plugin(args.algorithm);
            if (!plugin.handle) return 1;

            std::vector<uint8_t> key = load_key(args.key_file, args.generate_key, args.algorithm, args.save_key);
            if (key.empty()) return 1;

            process_crypto(plugin, key, args.mode == Mode::ENCRYPT,
                args.input_file, args.output_file);

            secure_zero(key.data(), key.size());
            unload_plugin(plugin);
            return 0;
        }

        std::cerr << "Invalid mode\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}