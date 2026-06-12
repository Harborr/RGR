#include "ecc.h"
#include "alphabet.h"
#include <vector>
#include <string>

// ========== Вспомогательные функции ==========

static std::string getMask(const std::vector<std::string>& chars) {
    std::string mask;
    for (const auto& ch : chars) mask += (isLower(ch) ? '1' : '0');
    return mask;
}

static std::string applyMask(const std::string& str, const std::string& mask) {
    if (str.size() != mask.size()) return str;
    std::vector<std::string> chars = utf8Split(str);
    std::string result;
    for (size_t i = 0; i < chars.size(); ++i) {
        std::string ch = chars[i];
        if (mask[i] == '1') ch = toLower(ch);
        result += ch;
    }
    return result;
}

static std::string toUpperString(const std::string& s) {
    std::vector<std::string> chars = utf8Split(s);
    std::string result;
    for (auto& ch : chars) result += toUpper(ch);
    return result;
}

// ========== Базовые функции (работают только с верхним регистром) ==========

static std::string eccEncryptUpper(const std::string& upperText, const std::string& upperKey) {
    if (upperKey.empty()) return upperText;
    std::vector<std::string> textChars = utf8Split(upperText);
    std::vector<std::string> keyChars = utf8Split(upperKey);
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

static std::string eccDecryptUpper(const std::string& upperCipher, const std::string& upperKey) {
    if (upperKey.empty()) return upperCipher;
    std::vector<std::string> cipherChars = utf8Split(upperCipher);
    std::vector<std::string> keyChars = utf8Split(upperKey);
    std::string result;
    for (size_t i = 0; i < cipherChars.size(); ++i) {
        int charIndex = getIndex(cipherChars[i]);
        if (charIndex >= 0) {
            int keyIndex = getIndex(keyChars[i % keyChars.size()]);
            if (keyIndex < 0) keyIndex = 0;
            int newIndex = (charIndex - (keyIndex * keyIndex)) % ALPHABET_SIZE;
            if (newIndex < 0) newIndex += ALPHABET_SIZE;
            result += ALPHABET[newIndex];
        } else {
            result += cipherChars[i];
        }
    }
    return result;
}

// ========== Публичные функции без маски (регистр не сохраняется) ==========

std::string eccEncrypt(const std::string& text, const std::string& key) {
    return eccEncryptUpper(toUpperString(text), toUpperString(key));
}

std::string eccDecrypt(const std::string& text, const std::string& key) {
    return eccDecryptUpper(toUpperString(text), toUpperString(key));
}

// ========== Публичные функции с маской (регистр сохраняется) ==========

std::string eccEncryptWithMask(const std::string& text, const std::string& key, std::string& mask) {
    if (key.empty()) {
        mask = std::string(text.size(), '0');
        return text;
    }
    std::vector<std::string> origChars = utf8Split(text);
    mask = getMask(origChars);
    std::string upperText = toUpperString(text);
    std::string upperKey = toUpperString(key);
    std::string encryptedUpper = eccEncryptUpper(upperText, upperKey);
    return applyMask(encryptedUpper, mask);
}

std::string eccDecryptWithMask(const std::string& text, const std::string& key, const std::string& mask) {
    if (key.empty()) return text;
    if (text.size() != mask.size()) return text;   // ошибка: длины не совпадают
    std::string upperCipher = toUpperString(text);
    std::string upperKey = toUpperString(key);
    std::string decryptedUpper = eccDecryptUpper(upperCipher, upperKey);
    return applyMask(decryptedUpper, mask);
}
