#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <string>

enum class Mode { NONE, ENCRYPT, DECRYPT, GENERATE_KEY };

struct Args {
    bool help = false;
    std::string algorithm;
    Mode mode = Mode::NONE;
    std::string key_file;
    bool generate_key = false;
    std::string save_key;
    bool write_key_stdout = false;
    std::string input_file;
    std::string output_file;
};

Args parse_args(int argc, char* argv[]);
void print_help();

#endif