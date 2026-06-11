#ifndef MENU_H
#define MENU_H

#include <string>
#include "../cipherAPI.h"

using namespace std;

class Menu {
public:
    void show();
private:
    void doText();
    void doFile();
    void doKey();
    string ask(const string& msg);
    ICipher* pickCiph();
    void pause();
};

#endif
