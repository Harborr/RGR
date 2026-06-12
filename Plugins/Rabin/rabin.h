#ifndef RABIN_H
#define RABIN_H

#include <string>

// Старые функции
std::string rabinEncrypt(const std::string& text, const std::string& key);
std::string rabinDecrypt(const std::string& text, const std::string& key);

// Новые функции с маской регистра
std::string rabinEncryptWithMask(const std::string& text, const std::string& key, std::string& mask);
std::string rabinDecryptWithMask(const std::string& text, const std::string& key, const std::string& mask);

#endif
