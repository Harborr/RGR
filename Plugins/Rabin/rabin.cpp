#include "rabin.h"
#include "alphabet.h"
#include <vector>
#include <string>

std::string rabinEncrypt(const std::string& text, const std::string& key) {
    if (key.empty()) return text;
    
    std::vector<std::string> textChars = utf8Split(text);
    std::vector<std::string> keyChars = utf8Split(key);
    std::string result;
    
    for (size_t i = 0; i < textChars.size(); ++i) {
        int charIndex = getIndex(textChars[i]);
        if (charIndex >= 0) {
            int keyIndex = getIndex(keyChars[i % keyChars.size()]);
            if (keyIndex < 0) keyIndex = 0;
            
            int newIndex = (charIndex * charIndex + keyIndex) % ALPHABET_SIZE;
            
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

std::string rabinDecrypt(const std::string& text, const std::string& key) {
    if (key.empty()) return text;
    
    std::vector<std::string> textChars = utf8Split(text);
    std::vector<std::string> keyChars = utf8Split(key);
    std::string result;
    
    for (size_t i = 0; i < textChars.size(); ++i) {
        int charIndex = getIndex(textChars[i]);
        if (charIndex >= 0) {
            int keyIndex = getIndex(keyChars[i % keyChars.size()]);
            if (keyIndex < 0) keyIndex = 0;
            
            int newIndex = -1;
            for (int x = 0; x < ALPHABET_SIZE; ++x) {
                if ((x * x + keyIndex) % ALPHABET_SIZE == charIndex) {
                    newIndex = x;
                    break;
                }
            }
            if (newIndex < 0) newIndex = charIndex;
            
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
