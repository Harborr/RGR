#include "xorencrypt.h"
#include "alphabet.h"
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

static bool checkKey(const string& key) {
    if (key.empty()) return false;
    if (key.size() < 3) return false;
    vector<string> kch = utf8Split(key);
    for (size_t i = 0; i < kch.size(); i++) {
        if (getIndex(kch[i]) < 0) return false;
    }
    return true;
}

static vector<int> keyToShift(const string& key) {
    vector<string> kch = utf8Split(key);
    vector<int> shifts;
    for (size_t i = 0; i < kch.size(); i++) {
        int idx = getIndex(kch[i]);
        shifts.push_back(idx);
    }
    return shifts;
}

string xorEncrypt(const string& text, const string& key) {
    if (text.empty()) return text;
    if (!checkKey(key)) throw runtime_error("XOR: bad key (min 3 sym, only alphabet)");

    vector<string> tch = utf8Split(text);
    vector<int> shifts = keyToShift(key);
    size_t ksz = shifts.size();
    string out;

    for (size_t i = 0; i < tch.size(); i++) {
        int ci = getIndex(tch[i]);
        if (ci >= 0) {
            int sh = shifts[i % ksz];
            int ni = (ci ^ sh) % ALPHABET_SIZE;
            if (ni < 0) ni += ALPHABET_SIZE;
            string ec = ALPHABET[ni];
            if (isLower(tch[i])) ec = toLower(ec);
            out += ec;
        } else {
            out += tch[i];
        }
    }

    return out;
}

string xorDecrypt(const string& text, const string& key) {
    if (text.empty()) return text;
    if (!checkKey(key)) throw runtime_error("XOR: bad key (min 3 sym, only alphabet)");
    return xorEncrypt(text, key);
}
