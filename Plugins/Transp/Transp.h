#ifndef TRANSP_H
#define TRANSP_H

#include "../../cipherAPI.h"

using namespace std;

class TranspCiph : public ICipher {
public:
    string name() const override;
    string encode(const string& txt, const string& key) override;
    string decode(const string& txt, const string& key) override;
    vector<uint8_t> encodeBin(const vector<uint8_t>& data, const string& key) override;
    vector<uint8_t> decodeBin(const vector<uint8_t>& data, const string& key) override;
    bool keyOk(const string& key) const override;
    string genKey() const override;
private:
    vector<int> getOrd(const string& key);
    string proc(const string& txt, const string& key, bool dir);
};

extern "C" {
    ICipher* createCipher();
    void destroyCipher(ICipher* c);
}

#endif
