#include "ecc.h"
#include "alphabet.h"
#include <vector>
#include <string>

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
            
            // ECC: (символ + квадрат ключа) mod размер алфавита
            int newIndex = (charIndex + (keyIndex * keyIndex)) % ALPHABET_SIZE;
            
            std::string encryptedChar = ALPHABET[newIndex];
            if (isLower(textChars[i])) {
                encryptedChar = toLower(encryptedChar);
            }
            result += encryptedChar;
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
            
            // Обратное преобразование: (символ - квадрат ключа) mod размер
            int newIndex = (charIndex - (keyIndex * keyIndex)) % ALPHABET_SIZE;
            if (newIndex < 0) newIndex += ALPHABET_SIZE;
            
            std::string decryptedChar = ALPHABET[newIndex];
            if (isLower(textChars[i])) {
                decryptedChar = toLower(decryptedChar);
            }
            result += decryptedChar;
        } else {
            result += textChars[i];
        }
    }
    return result;
}
