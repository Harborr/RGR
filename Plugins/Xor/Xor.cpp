#include "Xor.h"
#include <random>

string XorCiph::name() const { return "XOR"; }

string XorCiph::encode(const string& txt, const string& key) {
    string out;
    out.reserve(txt.size());
    for (size_t i = 0; i < txt.size(); i++)
        out += txt[i] ^ key[i % key.size()];
    return out;
}

string XorCiph::decode(const string& txt, const string& key) {
    return encode(txt, key);
}

vector<uint8_t> XorCiph::encodeBin(const vector<uint8_t>& data, const string& key) {
    vector<uint8_t> out;
    out.reserve(data.size());
    for (size_t i = 0; i < data.size(); i++)
        out.push_back(data[i] ^ (uint8_t)key[i % key.size()]);
    return out;
}

vector<uint8_t> XorCiph::decodeBin(const vector<uint8_t>& data, const string& key) {
    return encodeBin(data, key);
}

bool XorCiph::keyOk(const string& key) const {
    return !key.empty();
}

string XorCiph::genKey() const {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(33, 126);
    string k;
    for (int i = 0; i < 16; i++) k += (char)dis(gen);
    return k;
}

extern "C" ICipher* createCipher() { return new XorCiph(); }
extern "C" void destroyCipher(ICipher* c) { delete c; }
