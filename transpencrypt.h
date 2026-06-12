#ifndef TRANSPENCRYPT_H
#define TRANSPENCRYPT_H

#include <string>

using namespace std;

string transpEncrypt(const string& text, const string& key);
string transpDecrypt(const string& text, const string& key);

#endif
