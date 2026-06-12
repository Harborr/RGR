#ifndef ALPHABET_H
#define ALPHABET_H

#include <string>
#include <vector>

int utf8Len(unsigned char c);
std::vector<std::string> utf8Split(const std::string& s);

extern std::vector<std::string> ALPHABET;
extern int ALPHABET_SIZE;

void initAlphabet();
int getIndex(const std::string& ch);
bool isLower(const std::string& ch);
std::string toUpper(const std::string& ch);
std::string toLower(const std::string& ch);

#endif
