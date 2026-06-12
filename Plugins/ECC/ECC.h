#ifndef ECC_H
#define ECC_H

#include <string>

std::string eccEncrypt(const std::string& text, const std::string& key);
std::string eccDecrypt(const std::string& text, const std::string& key);

#endif
