#include <iostream>
#include <string>
#include "rabin.h"
#include "alphabet.h"

using namespace std;

int main() {
    initAlphabet();
    
    cout << "=== Rabin Шифр (отдельная программа) ===\n";
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
        string result = rabinEncrypt(all, key);
        cout << "\nРезультат:\n" << result << endl;
    } else if (choice == 2) {
        cout << "Текст для дешифрования (пустая строка - конец):\n";
        string line, all;
        while (getline(cin, line)) {
            if (line.empty()) break;
            if (!all.empty()) all += '\n';
            all += line;
        }
        string result = rabinDecrypt(all, key);
        cout << "\nРезультат:\n" << result << endl;
    } else {
        cout << "Неверный выбор.\n";
    }
    return 0;
}
