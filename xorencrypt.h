#ifndef XORENCRYPT_H
#define XORENCRYPT_H

#include <string>

using namespace std;

string xorEncrypt(const string& text, const string& key);
string xorDecrypt(const string& text, const string& key);

#endif
