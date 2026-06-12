#ifndef ECC_H
#define ECC_H

#include <string>

// Старые функции – для общей программы (регистр не сохраняется)
std::string eccEncrypt(const std::string& text, const std::string& key);
std::string eccDecrypt(const std::string& text, const std::string& key);

// Новые функции – для отдельной программы с сохранением регистра
// mask – выходная/входная строка из символов '0' (верхний регистр) и '1' (нижний регистр)
std::string eccEncryptWithMask(const std::string& text, const std::string& key, std::string& mask);
std::string eccDecryptWithMask(const std::string& text, const std::string& key, const std::string& mask);

#endif
