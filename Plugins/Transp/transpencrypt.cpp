#include "transpencrypt.h"
#include "alphabet.h"
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <stdexcept>

using namespace std;

static bool checkKey(const string& key) {
    if (key.empty()) return false;
    if (key.size() < 3) return false;
    vector<string> kch = utf8Split(key);
    for (size_t i = 0; i < kch.size(); i++) {
        for (size_t j = i + 1; j < kch.size(); j++) {
            if (kch[i] == kch[j]) return false;
        }
    }
    return true;
}

static vector<int> getOrd(const vector<string>& kch) {
    size_t sz = kch.size();
    vector<int> ord(sz);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&kch](int a, int b) {
        return kch[a] < kch[b];
    });
    return ord;
}

static vector<int> getInv(const vector<int>& ord) {
    size_t sz = ord.size();
    vector<int> inv(sz);
    for (size_t i = 0; i < sz; i++) inv[ord[i]] = i;
    return inv;
}

string transpEncrypt(const string& text, const string& key) {
    if (text.empty()) return text;
    if (!checkKey(key)) throw runtime_error("Transp: bad key (min 3 sym, unique, only alphabet)");

    vector<string> tch = utf8Split(text);
    vector<string> kch = utf8Split(key);

    size_t sz = kch.size();
    size_t blk = (tch.size() + sz - 1) / sz;
    size_t pad = blk * sz - tch.size();

    vector<string> padded = tch;
    for (size_t i = 0; i < pad; i++) {
        padded.push_back(ALPHABET[ALPHABET_SIZE - 1]);
    }

    vector<int> ord = getOrd(kch);
    vector<int> inv = getInv(ord);

    string out;

    for (size_t b = 0; b < blk; b++) {
        for (size_t i = 0; i < sz; i++) {
            out += padded[b * sz + inv[i]];
        }
    }

    return out;
}

string transpDecrypt(const string& text, const string& key) {
    if (text.empty()) return text;
    if (!checkKey(key)) throw runtime_error("Transp: bad key (min 3 sym, unique, only alphabet)");

    vector<string> tch = utf8Split(text);
    vector<string> kch = utf8Split(key);

    size_t sz = kch.size();

    if (tch.size() % sz != 0) throw runtime_error("Transp: corrupted ciphertext");

    size_t blk = tch.size() / sz;

    vector<int> ord = getOrd(kch);

    string out;

    for (size_t b = 0; b < blk; b++) {
        vector<string> block(sz, "");
        for (size_t i = 0; i < sz; i++) {
            block[i] = tch[b * sz + i];
        }
        vector<string> orig(sz, "");
        for (size_t i = 0; i < sz; i++) {
            orig[ord[i]] = block[i];
        }
        for (size_t i = 0; i < sz; i++) {
            out += orig[i];
        }
    }

    return out;
}
