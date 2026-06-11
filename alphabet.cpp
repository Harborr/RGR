#include "alphabet.h"
#include <cstring>

int utf8Len(unsigned char c) {
    if (c < 0x80) return 1;
    if (c < 0xC0) return 1;
    if (c < 0xE0) return 2;
    if (c < 0xF0) return 3;
    return 4;
}

std::vector<std::string> utf8Split(const std::string& s) {
    std::vector<std::string> v;
    for (size_t i = 0; i < s.size();) {
        int len = utf8Len((unsigned char)s[i]);
        v.push_back(s.substr(i, len));
        i += len;
    }
    return v;
}

static const char* RU_TABLE[][2] = {
    {"а","А"},{"б","Б"},{"в","В"},{"г","Г"},{"д","Д"},
    {"е","Е"},{"ё","Ё"},{"ж","Ж"},{"з","З"},{"и","И"},
    {"й","Й"},{"к","К"},{"л","Л"},{"м","М"},{"н","Н"},
    {"о","О"},{"п","П"},{"р","Р"},{"с","С"},{"т","Т"},
    {"у","У"},{"ф","Ф"},{"х","Х"},{"ц","Ц"},{"ч","Ч"},
    {"ш","Ш"},{"щ","Щ"},{"ъ","Ъ"},{"ы","Ы"},{"ь","Ь"},
    {"э","Э"},{"ю","Ю"},{"я","Я"}
};

std::string toUpper(const std::string& ch) {
    if (ch.size() == 1 && ch[0] >= 'a' && ch[0] <= 'z') {
        return std::string(1, ch[0] - 32);
    }
    if (ch.size() == 1 && ch[0] >= 'A' && ch[0] <= 'Z') {
        return ch;
    }
    for (int i = 0; i < 33; i++) {
        if (ch == RU_TABLE[i][0]) return RU_TABLE[i][1];
        if (ch == RU_TABLE[i][1]) return ch;
    }
    return ch;
}

std::string toLower(const std::string& ch) {
    if (ch.size() == 1 && ch[0] >= 'A' && ch[0] <= 'Z') {
        return std::string(1, ch[0] + 32);
    }
    if (ch.size() == 1 && ch[0] >= 'a' && ch[0] <= 'z') {
        return ch;
    }
    for (int i = 0; i < 33; i++) {
        if (ch == RU_TABLE[i][1]) return RU_TABLE[i][0];
        if (ch == RU_TABLE[i][0]) return ch;
    }
    return ch;
}

bool isLower(const std::string& ch) {
    if (ch.size() == 1 && ch[0] >= 'a' && ch[0] <= 'z') return true;
    for (int i = 0; i < 33; i++) {
        if (ch == RU_TABLE[i][0]) return true;
    }
    return false;
}

static const std::string ALPHABET_STRING =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
    "0123456789"
    " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~\t\n\r";

std::vector<std::string> ALPHABET;
int ALPHABET_SIZE = 0;

void initAlphabet() {
    ALPHABET = utf8Split(ALPHABET_STRING);
    ALPHABET_SIZE = (int)ALPHABET.size();
}

int getIndex(const std::string& ch) {
    std::string upper = toUpper(ch);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (ALPHABET[i] == upper) return i;
    }
    return -1;
}
