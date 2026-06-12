#include <iostream>
#include <string>
#include "ecc.h"
#include "alphabet.h"

using namespace std;

int main() {
    initAlphabet();
    
    cout << "=== ECC Шифр (отдельная программа) ===\n";
    cout << "1. Зашифровать\n2. Расшифровать\nВыбор: ";
    int choice;
    cin >> choice;
    cin.ignore();
    
    string key;
    cout << "Ключ: ";
    getline(cin, key);
    
    if (choice == 1) {
        cout << "Текст для шифрования (пустая строка - конец):\n";
        string line, all;
        while (getline(cin, line)) {
            if (line.empty()) break;
            if (!all.empty()) all += '\n';
            all += line;
        }
        string result = eccEncrypt(all, key);
        cout << "\nРезультат:\n" << result << endl;
    } else if (choice == 2) {
        cout << "Текст для дешифрования (пустая строка - конец):\n";
        string line, all;
        while (getline(cin, line)) {
            if (line.empty()) break;
            if (!all.empty()) all += '\n';
            all += line;
        }
        string result = eccDecrypt(all, key);
        cout << "\nРезультат:\n" << result << endl;
    } else {
        cout << "Неверный выбор.\n";
    }
    return 0;
}
