#ifndef ECC_H
#define ECC_H

#include <string>

// Обычные функции (без сохранения регистра) — для общей программы
std::string eccEncrypt(const std::string& text, const std::string& key);
std::string eccDecrypt(const std::string& text, const std::string& key);

// Функции с маской (сохраняют регистр) — для отдельной программы
std::string eccEncryptWithMask(const std::string& text, const std::string& key, std::string& mask);
std::string eccDecryptWithMask(const std::string& text, const std::string& key, const std::string& mask);

#endif
