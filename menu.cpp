#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>  // Для numeric_limits
#include "alphabet.h"
#include "vigenere.h"
#include "hill.h"

using namespace std;

string input(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

string multiline(const string& prompt) {
    cout << prompt << "\n(пустая строка — конец ввода)\n";
    string all, line;
    while (true) {
        getline(cin, line);
        if (line.empty()) break;
        if (!all.empty()) all += "\n";
        all += line;
    }
    return all;
}

void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void showMenu() {
    cout << "\n==================================================\n";
    cout << "   ПРОГРАММА ШИФРОВАНИЯ\n";
    cout << "==================================================\n";
    cout << "1. Шифр Виженера\n";
    cout << "2. Hill Cipher\n";
    cout << "3. О программе\n";
    cout << "0. Выход\n";
    cout << "==================================================\n";
    cout << "Ваш выбор: ";
}

int opMenu(const string& name) {
    cout << "\n--- " << name << " ---\n";
    cout << "1. Зашифровать\n2. Расшифровать\n3. Назад\nВаш выбор: ";
    int c; 
    cin >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера после ввода числа
    return c;
}

void vigenereMenu() {
    while (true) {
        cls();
        int c = opMenu("ШИФР ВИЖЕНЕРА");
        if (c == 3) break;
        
        // Важно: ignore уже сделан в opMenu, здесь не нужно
        string key = input("Ключ: ");
        
        if (c == 1) {
            string pt = multiline("Текст для шифрования:");
            cout << "\nРезультат:\n" << vigenereEncrypt(pt, key) << "\n";
        } else if (c == 2) {
            string ct = multiline("Текст для дешифрования:");
            cout << "\nРезультат:\n" << vigenereDecrypt(ct, key) << "\n";
        } else {
            continue; // Неверный выбор - возвращаемся в меню
        }
        cout << "\nНажмите Enter для продолжения..."; 
        cin.get(); // Ждём Enter
    }
}

void hillMenu() {
    while (true) {
        cls();
        int c = opMenu("HILL CIPHER");
        if (c == 3) break;
        
        string key = input("Ключ-матрица: ");
        
        if (c == 1) {
            string pt = multiline("Текст для шифрования:");
            string r = hillEncrypt(pt, key);
            if (!r.empty()) cout << "\nРезультат:\n" << r << "\n";
        } else if (c == 2) {
            string ct = multiline("Текст для дешифрования:");
            string r = hillDecrypt(ct, key);
            if (!r.empty()) cout << "\nРезультат:\n" << r << "\n";
        } else {
            continue;
        }
        cout << "\nНажмите Enter для продолжения..."; 
        cin.get();
    }
}

void about() {
    cls();
    cout << "\nПрограмма шифрования v3.0\n";
    cout << "Поддерживает:\n";
    cout << "  - Шифр Виженера\n";
    cout << "  - Hill Cipher\n";
    cout << "  - Латиницу, кириллицу, цифры, спецсимволы\n";
    cout << "Размер алфавита: " << ALPHABET_SIZE << " символов\n";
    cout << "\nНажмите Enter для продолжения..."; 
    cin.get();
}

int main() {
    initAlphabet();
    while (true) {
        cls(); 
        showMenu();
        int c; 
        cin >> c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
        
        switch (c) {
            case 1: vigenereMenu(); break;
            case 2: hillMenu(); break;
            case 3: about(); break;
            case 0: return 0;
        }
    }
}
