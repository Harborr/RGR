#include <iostream>
#include <string>
#include "rabin.h"
#include "alphabet.h"

using namespace std;

int main() {
    initAlphabet();
    
    cout << "=== Rabin Шифр (с сохранением регистра) ===\n";
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
        string mask;
        string encrypted = rabinEncryptWithMask(all, key, mask);
        cout << "\nЗашифрованный текст:\n" << encrypted << "\n";
        cout << "Маска регистра (0=верхний,1=нижний):\n" << mask << "\n";
        cout << "\nСохраните оба значения для последующего расшифрования.\n";
    } 
    else if (choice == 2) {
        cout << "Введите зашифрованный текст:\n";
        string cipher;
        getline(cin, cipher);
        cout << "Введите маску регистра (строка из 0 и 1 той же длины):\n";
        string mask;
        getline(cin, mask);
        string decrypted = rabinDecryptWithMask(cipher, key, mask);
        cout << "\nРезультат расшифрования:\n" << decrypted << "\n";
    } 
    else {
        cout << "Неверный выбор.\n";
    }
    return 0;
}
