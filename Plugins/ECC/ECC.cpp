#include "Transp.h"
#include <algorithm>
#include <numeric>
#include <random>

string TranspCiph::name() const { return "Transp"; }

vector<int> TranspCiph::getOrd(const string& key) {
    vector<int> ord(key.size());
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&key](int a, int b) { return key[a] < key[b]; });
    return ord;
}

string TranspCiph::proc(const string& txt, const string& key, bool dir) {
    if (txt.empty() || key.empty()) return txt;
    size_t sz = key.size();
    size_t blk = (txt.size() + sz - 1) / sz;
    string out;
    out.reserve(txt.size());
    vector<int> ord = getOrd(key);
    if (dir) {
        vector<int> inv(ord.size());
        for (size_t i = 0; i < ord.size(); i++) inv[ord[i]] = i;
        ord = inv;
    }
    for (size_t b = 0; b < blk; b++) {
        string blkTxt(sz, '\0');
        for (size_t i = 0; i < sz; i++) {
            size_t pos = b * sz + i;
            if (pos < txt.size()) blkTxt[i] = txt[pos];
        }
        string prm(sz, '\0');
        for (size_t i = 0; i < sz; i++) prm[ord[i]] = blkTxt[i];
        out += prm;
    }
    out.resize(txt.size());
    return out;
}

string TranspCiph::encode(const string& txt, const string& key) { return proc(txt, key, true); }
string TranspCiph::decode(const string& txt, const string& key) { return proc(txt, key, false); }

vector<uint8_t> TranspCiph::encodeBin(const vector<uint8_t>& data, const string& key) {
    string s(data.begin(), data.end());
    string r = encode(s, key);
    return vector<uint8_t>(r.begin(), r.end());
}

vector<uint8_t> TranspCiph::decodeBin(const vector<uint8_t>& data, const string& key) {
    string s(data.begin(), data.end());
    string r = decode(s, key);
    return vector<uint8_t>(r.begin(), r.end());
}

bool TranspCiph::keyOk(const string& key) const { return key.size() >= 2; }

string TranspCiph::genKey() const {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> ldis(4, 10);
    int len = ldis(gen);
    uniform_int_distribution<> cdis('A', 'Z');
    string k;
    for (int i = 0; i < len; i++) k += (char)cdis(gen);
    return k;
}

extern "C" ICipher* createCipher() { return new TranspCiph(); }
extern "C" void destroyCipher(ICipher* c) { delete c; }
