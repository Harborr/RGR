#include "xorencrypt.h"
#include "alphabet.h"
#include <vector>
#include <string>

using namespace std;

string xorEncrypt(const string& text, const string& key) {
    if (key.empty()) return text;
    
    vector<string> tch = utf8Split(text);
    vector<string> kch = utf8Split(key);
    string out;
    
    for (size_t i = 0; i < tch.size(); i++) {
        int ci = getIndex(tch[i]);
        
        if (ci >= 0) {
            int ki = getIndex(kch[i % kch.size()]);
            if (ki < 0) ki = 0;
            
            int ni = (ci ^ ki) % ALPHABET_SIZE;
            
            string ec = ALPHABET[ni];
            
            if (isLower(tch[i])) {
                ec = toLower(ec);
            }
            
            out += ec;
        } else {
            out += tch[i];
        }
    }
    
    return out;
}

string xorDecrypt(const string& text, const string& key) {
    return xorEncrypt(text, key);
}
