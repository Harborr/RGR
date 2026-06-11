#include "hill.h"
#include "alphabet.h"
#include <vector>
#include <iostream>
#include <string>

static int egcd(int a, int b, int& x, int& y) {
    if (!b) { x=1; y=0; return a; }
    int x1,y1;
    int g = egcd(b, a%b, x1, y1);
    x = y1;
    y = x1 - (a/b)*y1;
    return g;
}

static int invmod(int a, int m) {
    int x,y;
    if (egcd(a,m,x,y) != 1) return -1;
    return (x%m + m) % m;
}

static bool inv2(const std::vector<std::vector<int>>& m,
                 std::vector<std::vector<int>>& inv) {
    int det = (m[0][0]*m[1][1] - m[0][1]*m[1][0]) % ALPHABET_SIZE;
    det = (det + ALPHABET_SIZE) % ALPHABET_SIZE;
    int idet = invmod(det, ALPHABET_SIZE);
    if (idet == -1) return false;
    
    inv.clear();
    inv.resize(2, std::vector<int>(2, 0));
    
    inv[0][0] = (( m[1][1]*idet) % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;
    inv[0][1] = ((-m[0][1]*idet) % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;
    inv[1][0] = ((-m[1][0]*idet) % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;
    inv[1][1] = (( m[0][0]*idet) % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;
    return true;
}

static std::vector<std::vector<int>> makeKey(const std::string& key) {
    auto kc = utf8Split(key);
    
    std::vector<std::vector<int>> m;
    m.resize(2, std::vector<int>(2, 0));
    
    int p = 0;
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++) {
            if (p < (int)kc.size()) {
                int idx = getIndex(kc[p]);
                m[i][j] = (idx>=0) ? idx : p%ALPHABET_SIZE;
                p++;
            } else m[i][j] = p++ % ALPHABET_SIZE;
        }
    return m;
}

static std::vector<int> mul(const std::vector<std::vector<int>>& m,
                            const std::vector<int>& v) {
    std::vector<int> r;
    r.resize(2, 0);
    
    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++)
            r[i] = (r[i] + m[i][j]*v[j]) % ALPHABET_SIZE;
        r[i] = (r[i]+ALPHABET_SIZE) % ALPHABET_SIZE;
    }
    return r;
}

std::string hillEncrypt(const std::string& text, const std::string& key) {
    if (key.empty()) return "";
    auto km = makeKey(key);
    std::vector<std::vector<int>> inv;
    if (!inv2(km, inv)) {
        std::cerr << "Ключ не подходит (матрица необратима). Попробуйте: HILL, MATH, CODE\n";
        return "";
    }
    auto tc = utf8Split(text);
    
    // Сохраняем регистр
    std::vector<bool> lowerMask;
    for (auto& ch : tc) {
        lowerMask.push_back(isLower(ch));
    }
    
    if (tc.size() % 2) {
        tc.push_back(" ");
        lowerMask.push_back(false);
    }
    
    std::string out;
    for (size_t i=0; i<tc.size(); i+=2) {
        int a = getIndex(tc[i]);   if (a<0) a=0;
        int b = getIndex(tc[i+1]); if (b<0) b=0;
        
        std::vector<int> vec;
        vec.push_back(a);
        vec.push_back(b);
        
        auto r = mul(km, vec);
        
        std::string ch1 = ALPHABET[r[0]];
        std::string ch2 = ALPHABET[r[1]];
        
        // Сохраняем регистр
        if (lowerMask[i]) ch1 = toLower(ch1);
        if (lowerMask[i+1]) ch2 = toLower(ch2);
        
        out += ch1 + ch2;
    }
    return out;
}

std::string hillDecrypt(const std::string& text, const std::string& key) {
    if (key.empty()) return "";
    auto km = makeKey(key);
    std::vector<std::vector<int>> inv;
    if (!inv2(km, inv)) return "";
    auto tc = utf8Split(text);
    if (tc.size() % 2) return "";
    
    // Сохраняем регистр зашифрованного текста
    std::vector<bool> lowerMask;
    for (auto& ch : tc) {
        lowerMask.push_back(isLower(ch));
    }
    
    std::string out;
    for (size_t i=0; i<tc.size(); i+=2) {
        int a = getIndex(tc[i]);
        int b = getIndex(tc[i+1]);
        
        std::vector<int> vec;
        vec.push_back(a);
        vec.push_back(b);
        
        auto r = mul(inv, vec);
        
        std::string ch1 = ALPHABET[r[0]];
        std::string ch2 = ALPHABET[r[1]];
        
        // Восстанавливаем регистр
        if (lowerMask[i]) ch1 = toLower(ch1);
        if (lowerMask[i+1]) ch2 = toLower(ch2);
        
        out += ch1 + ch2;
    }
    return out;
}
