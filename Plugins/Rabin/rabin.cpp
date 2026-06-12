#include "rabin.h"
#include "alphabet.h"
#include <vector>
#include <string>

static std::string getMask(const std::vector<std::string>& chars) {
    std::string mask;
    for (auto& ch : chars) mask += (isLower(ch) ? '1' : '0');
    return mask;
}

// Старые функции (без регистра)
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
            result += ALPHABET[newIndex];
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
            result += ALPHABET[newIndex];
        } else {
            result += textChars[i];
        }
    }
    return result;
}

// Новые функции с маской
std::string rabinEncryptWithMask(const std::string& text, const std::string& key, std::string& mask) {
    if (key.empty()) { mask = std::string(text.size(), '0'); return text; }
    
    std::vector<std::string> textChars = utf8Split(text);
    std::vector<std::string> keyChars = utf8Split(key);
    mask = getMask(textChars);
    
    std::string result;
    for (size_t i = 0; i < textChars.size(); ++i) {
        int charIndex = getIndex(textChars[i]);
        if (charIndex >= 0) {
            int keyIndex = getIndex(keyChars[i % keyChars.size()]);
            if (keyIndex < 0) keyIndex = 0;
            
            int newIndex = (charIndex * charIndex + keyIndex) % ALPHABET_SIZE;
            std::string enc = ALPHABET[newIndex];
            if (mask[i] == '1') enc = toLower(enc);
            result += enc;
        } else {
            result += textChars[i];
        }
    }
    return result;
}

std::string rabinDecryptWithMask(const std::string& text, const std::string& key, const std::string& mask) {
    if (key.empty()) return text;
    if (text.size() != mask.size()) return text;
    
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
            std::string dec = ALPHABET[newIndex];
            if (mask[i] == '1') dec = toLower(dec);
            result += dec;
        } else {
            result += textChars[i];
        }
    }
    return result;
}
