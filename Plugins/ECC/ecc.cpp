#include "ecc.h"
#include "alphabet.h"
#include <vector>
#include <string>

// Вспомогательная функция: получить маску регистра для строки
static std::string getMask(const std::vector<std::string>& chars) {
    std::string mask;
    for (auto& ch : chars) {
        mask += (isLower(ch) ? '1' : '0');
    }
    return mask;
}

// Старые функции (без сохранения регистра) – для общей программы
std::string eccEncrypt(const std::string& text, const std::string& key) {
    if (key.empty()) return text;
    
    std::vector<std::string> textChars = utf8Split(text);
    std::vector<std::string> keyChars = utf8Split(key);
    std::string result;
    
    for (size_t i = 0; i < textChars.size(); ++i) {
        int charIndex = getIndex(textChars[i]);
        if (charIndex >= 0) {
            int keyIndex = getIndex(keyChars[i % keyChars.size()]);
            if (keyIndex < 0) keyIndex = 0;
            
            int newIndex = (charIndex + (keyIndex * keyIndex)) % ALPHABET_SIZE;
            result += ALPHABET[newIndex];
        } else {
            result += textChars[i];
        }
    }
    return result;
}

std::string eccDecrypt(const std::string& text, const std::string& key) {
    if (key.empty()) return text;
    
    std::vector<std::string> textChars = utf8Split(text);
    std::vector<std::string> keyChars = utf8Split(key);
    std::string result;
    
    for (size_t i = 0; i < textChars.size(); ++i) {
        int charIndex = getIndex(textChars[i]);
        if (charIndex >= 0) {
            int keyIndex = getIndex(keyChars[i % keyChars.size()]);
            if (keyIndex < 0) keyIndex = 0;
            
            int newIndex = (charIndex - (keyIndex * keyIndex)) % ALPHABET_SIZE;
            if (newIndex < 0) newIndex += ALPHABET_SIZE;
            result += ALPHABET[newIndex];
        } else {
            result += textChars[i];
        }
    }
    return result;
}

// Новые функции с маской регистра (для отдельной программы)
std::string eccEncryptWithMask(const std::string& text, const std::string& key, std::string& mask) {
    if (key.empty()) { mask = std::string(text.size(), '0'); return text; }
    
    std::vector<std::string> textChars = utf8Split(text);
    std::vector<std::string> keyChars = utf8Split(key);
    mask = getMask(textChars);  // запоминаем регистр исходного текста
    
    std::string result;
    for (size_t i = 0; i < textChars.size(); ++i) {
        int charIndex = getIndex(textChars[i]);
        if (charIndex >= 0) {
            int keyIndex = getIndex(keyChars[i % keyChars.size()]);
            if (keyIndex < 0) keyIndex = 0;
            
            int newIndex = (charIndex + (keyIndex * keyIndex)) % ALPHABET_SIZE;
            std::string enc = ALPHABET[newIndex];
            
            // Применяем регистр в соответствии с маской
            if (mask[i] == '1') enc = toLower(enc);
            result += enc;
        } else {
            result += textChars[i];
            // Для не-букв маска уже установлена в '0' (условно)
        }
    }
    return result;
}

std::string eccDecryptWithMask(const std::string& text, const std::string& key, const std::string& mask) {
    if (key.empty()) return text;
    if (text.size() != mask.size()) return text; // Ошибка: маска не совпадает по длине
    
    std::vector<std::string> textChars = utf8Split(text);
    std::vector<std::string> keyChars = utf8Split(key);
    
    std::string result;
    for (size_t i = 0; i < textChars.size(); ++i) {
        int charIndex = getIndex(textChars[i]);
        if (charIndex >= 0) {
            int keyIndex = getIndex(keyChars[i % keyChars.size()]);
            if (keyIndex < 0) keyIndex = 0;
            
            int newIndex = (charIndex - (keyIndex * keyIndex)) % ALPHABET_SIZE;
            if (newIndex < 0) newIndex += ALPHABET_SIZE;
            std::string dec = ALPHABET[newIndex];
            
            // Восстанавливаем регистр по маске
            if (mask[i] == '1') dec = toLower(dec);
            result += dec;
        } else {
            result += textChars[i];
        }
    }
    return result;
}
