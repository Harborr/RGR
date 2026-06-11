#ifndef CIPHER_API_H
#define CIPHER_API_H

#include <string>
#include <vector>
#include <cstdint>

using namespace std;

class ICipher {
public:
    virtual ~ICipher() {}
    virtual string name() const = 0;
    virtual string encode(const string& txt, const string& key) = 0;
    virtual string decode(const string& txt, const string& key) = 0;
    virtual vector<uint8_t> encodeBin(const vector<uint8_t>& data, const string& key) = 0;
    virtual vector<uint8_t> decodeBin(const vector<uint8_t>& data, const string& key) = 0;
    virtual bool keyOk(const string& key) const = 0;
    virtual string genKey() const = 0;
};

typedef ICipher* (*MakeCipher)();
typedef void (*KillCipher)(ICipher*);

#endif
