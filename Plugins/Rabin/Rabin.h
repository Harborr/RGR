#ifndef RABIN_H
#define RABIN_H

#include <string>

std::string rabinEncrypt(const std::string& text, const std::string& key);
std::string rabinDecrypt(const std::string& text, const std::string& key);

#endif
