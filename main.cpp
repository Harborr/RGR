#include "UI/Menu.h"
#include <iostream>
#include <locale>

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    cout << "\n  ______                     _   _             " << endl;
    cout << " |  ____|                   | | (_)            " << endl;
    cout << " | |__   _ __   ___ _ __ ___| |_ _  ___  _ __  " << endl;
    cout << " |  __| | '_ \\ / __| '__/ __| __| |/ _ \\| '_ \\ " << endl;
    cout << " | |____| | | | (__| | | (__| |_| | (_) | | | |" << endl;
    cout << " |______|_| |_|\\___|_|  \\___|\\__|_|\\___/|_| |_|" << endl;
    cout << endl;
    cout << "  Шифровальщик РГР v1.0" << endl;
    cout << "  =======================" << endl;
    Menu m;
    m.show();
    return 0;
}
