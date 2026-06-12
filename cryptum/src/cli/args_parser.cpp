#include "args_parser.h"
#include <iostream>
#include <getopt.h>

Args parse_args(int argc, char* argv[]) {
    Args args;
    static option long_opts[] = {
        {"help", no_argument, 0, 'h'},
        {"algorithm", required_argument, 0, 'a'},
        {"mode", required_argument, 0, 'm'},
        {"key", required_argument, 0, 'k'},
        {"generate-key", no_argument, 0, 'g'},
        {"save-key", required_argument, 0, 's'},
        {"write-key", no_argument, 0, 'w'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0,0,0,0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "ha:m:k:gws:i:o:", long_opts, nullptr)) != -1) {
        switch (opt) {
        case 'h': args.help = true; break;
        case 'a': args.algorithm = optarg; break;
        case 'm':
            if (std::string(optarg) == "encrypt") args.mode = Mode::ENCRYPT;
            else if (optarg == std::string("decrypt")) args.mode = Mode::DECRYPT;
            else if (optarg == std::string("generate-key")) args.mode = Mode::GENERATE_KEY;
            break;
        case 'k': args.key_file = optarg; break;
        case 'g': args.generate_key = true; break;
        case 's': args.save_key = optarg; break;
        case 'w': args.write_key_stdout = true; break;
        case 'i': args.input_file = optarg; break;
        case 'o': args.output_file = optarg; break;
        default: break;
        }
    }
    return args;
}

void print_help() {
    std::cout << "Usage: cryptum [options]\n"
        << "Options:\n"
        << "  -h, --help                 Show help\n"
        << "  -a, --algorithm <name>     Algorithm: vigenere, hill, xor_cipher, transposition, ecc, rabin\n"
        << "  -m, --mode <mode>          encrypt, decrypt, generate-key\n"
        << "  -k, --key <file>           Key file\n"
        << "  -g, --generate-key         Generate random key\n"
        << "  -s, --save-key <file>      Save generated key\n"
        << "  -w, --write-key            Write key to stdout\n"
        << "  -i, --input <file>         Input file (default stdin)\n"
        << "  -o, --output <file>        Output file (default stdout)\n";
}