#include "vigenere.h"
#include "alphabet.h"
#include <iostream>

std::string vigenereEncrypt(const std::string& text, const std::string& key) {
    if (key.empty() || text.empty()) return text;
    
    auto t = utf8Split(text);
    auto k = utf8Split(key);
    
    std::string out;
    size_t ki = 0;
    
    for (auto& ch : t) {
        int ci = getIndex(ch);
        if (ci >= 0) {
            int ki2 = getIndex(k[ki % k.size()]);
            if (ki2 < 0) ki2 = 0;
            
            std::string encrypted = ALPHABET[(ci + ki2) % ALPHABET_SIZE];
            
            // Сохраняем регистр: если исходная буква была строчной,
            // делаем зашифрованную тоже строчной
            if (isLower(ch)) {
                encrypted = toLower(encrypted);
            }
            
            out += encrypted;
            ki++;
        } else {
            out += ch;
        }
    }
    return out;
}

std::string vigenereDecrypt(const std::string& text, const std::string& key) {
    if (key.empty() || text.empty()) return text;
    
    auto t = utf8Split(text);
    auto k = utf8Split(key);
    
    std::string out;
    size_t ki = 0;
    
    for (auto& ch : t) {
        int ci = getIndex(ch);  // getIndex приводит к верхнему регистру
        if (ci >= 0) {
            int ki2 = getIndex(k[ki % k.size()]);
            if (ki2 < 0) ki2 = 0;
            
            std::string decrypted = ALPHABET[(ci - ki2 + ALPHABET_SIZE) % ALPHABET_SIZE];
            
            // Восстанавливаем регистр по зашифрованному символу
            if (isLower(ch)) {
                decrypted = toLower(decrypted);
            }
            
            out += decrypted;
            ki++;
        } else {
            out += ch;
        }
    }
    return out;
}
