/*
 * Главная программа-меню для шифрования/дешифрования
 * С динамическим определением доступных модулей
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <vector>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

#include "alphabet.h"

// ====== БАЗОВЫЕ МОДУЛИ (всегда есть) ======
#include "vigenere.h"
#include "hill.h"

// ====== ДОПОЛНИТЕЛЬНЫЕ МОДУЛИ (с проверкой) ======
#ifdef XOR_AVAILABLE
    #include "Xor.h"
#else
    // Заглушки
    #include <string>
    inline std::string XorEncrypt(const std::string&, const std::string&) { return ""; }
    inline std::string XorDecrypt(const std::string&, const std::string&) { return ""; }
#endif

#ifdef TRANSP_AVAILABLE
    #include "transp.h"
#else
    #include <string>
    inline std::string transpEncrypt(const std::string&, const std::string&) { return ""; }
    inline std::string transpDecrypt(const std::string&, const std::string&) { return ""; }
#endif

#ifdef RABIN_AVAILABLE
    #include "rabin.h"
#else
    #include <string>
    inline std::string rabinEncrypt(const std::string&, const std::string&) { return ""; }
    inline std::string rabinDecrypt(const std::string&, const std::string&) { return ""; }
#endif

#ifdef ECC_AVAILABLE
    #include "ecc.h"
#else
    #include <string>
    inline std::string eccEncrypt(const std::string&, const std::string&) { return ""; }
    inline std::string eccDecrypt(const std::string&, const std::string&) { return ""; }
#endif

using namespace std;

// ====== СТРУКТУРА ДЛЯ ХРАНЕНИЯ ИНФОРМАЦИИ О МОДУЛЕ ======
struct CipherModule {
    string name;           // Название для меню
    string keyPrompt;      // Подсказка для ввода ключа
    string (*encrypt)(const string&, const string&);  // Указатель на функцию шифрования
    string (*decrypt)(const string&, const string&);  // Указатель на функцию дешифрования
    bool available;        // Доступен ли модуль
};

// ====== СПИСОК ВСЕХ МОДУЛЕЙ ======
vector<CipherModule> modules;

void registerModules() {
    modules.clear();
    
    // Модули, которые есть всегда
    modules.push_back({
        "ШИФР ВИЖЕНЕРА",
        "Ключ: ",
        vigenereEncrypt,
        vigenereDecrypt,
        true
    });
    
    modules.push_back({
        "HILL CIPHER",
        "Ключ-матрица: ",
        hillEncrypt,
        hillDecrypt,
        true
    });
    
    // Дополнительные модули
#ifdef XOR_AVAILABLE
    modules.push_back({
        "XOR ШИФР",
        "Ключ: ",
        XorEncrypt,
        XorDecrypt,
        true
    });
#else
    modules.push_back({
        "XOR ШИФР (НЕДОСТУПЕН)",
        "",
        nullptr,
        nullptr,
        false
    });
#endif

#ifdef TRANSP_AVAILABLE
    modules.push_back({
        "ШИФР ПЕРЕСТАНОВКИ",
        "Ключ: ",
        transpEncrypt,
        transpDecrypt,
        true
    });
#else
    modules.push_back({
        "ШИФР ПЕРЕСТАНОВКИ (НЕДОСТУПЕН)",
        "",
        nullptr,
        nullptr,
        false
    });
#endif

#ifdef RABIN_AVAILABLE
    modules.push_back({
        "ШИФР РАБИНА",
        "Ключ: ",
        rabinEncrypt,
        rabinDecrypt,
        true
    });
#else
    modules.push_back({
        "ШИФР РАБИНА (НЕДОСТУПЕН)",
        "",
        nullptr,
        nullptr,
        false
    });
#endif

#ifdef ECC_AVAILABLE
    modules.push_back({
        "ECC (ЭЛЛИПТИЧЕСКИЕ КРИВЫЕ)",
        "Ключ: ",
        eccEncrypt,
        eccDecrypt,
        true
    });
#else
    modules.push_back({
        "ECC ЭЛЛИПТИЧЕСКИЕ КРИВЫЕ (НЕДОСТУПЕН)",
        "",
        nullptr,
        nullptr,
        false
    });
#endif
}

// ====== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ======
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
    system(CLEAR_SCREEN);
}

void showMenu() {
    cout << "\n==================================================\n";
    cout << "   ПРОГРАММА ШИФРОВАНИЯ И ДЕШИФРОВАНИЯ\n";
    cout << "==================================================\n";
    
    for (size_t i = 0; i < modules.size(); i++) {
        cout << (i + 1) << ". " << modules[i].name << "\n";
    }
    
    cout << modules.size() + 1 << ". О программе\n";
    cout << "0. Выход\n";
    cout << "==================================================\n";
    cout << "Ваш выбор: ";
}

int opMenu(const string& name) {
    cout << "\n--- " << name << " ---\n";
    cout << "1. Зашифровать\n2. Расшифровать\n3. Назад\nВаш выбор: ";
    int c; 
    cin >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return c;
}

// ====== УНИВЕРСАЛЬНАЯ ФУНКЦИЯ МЕНЮ ДЛЯ ЛЮБОГО МОДУЛЯ ======
void cipherMenu(int index) {
    CipherModule& mod = modules[index];
    
    if (!mod.available) {
        cout << "\nЭтот модуль не подключен!\n";
        cout << "Добавьте файлы модуля и перекомпилируйте программу.\n";
        cout << "\nНажмите Enter..."; 
        cin.get();
        return;
    }
    
    while (true) {
        cls();
        int c = opMenu(mod.name);
        if (c == 3) break;
        
        string key = input(mod.keyPrompt);
        
        if (c == 1) {
            string pt = multiline("Текст для шифрования:");
            string r = mod.encrypt(pt, key);
            if (!r.empty()) cout << "\nРезультат:\n" << r << "\n";
        } else if (c == 2) {
            string ct = multiline("Текст для дешифрования:");
            string r = mod.decrypt(ct, key);
            if (!r.empty()) cout << "\nРезультат:\n" << r << "\n";
        }
        cout << "\nНажмите Enter для продолжения..."; 
        cin.get();
    }
}

// ====== ИНФОРМАЦИЯ О ПРОГРАММЕ ======
void about() {
    cls();
    cout << "\n==================================================\n";
    cout << "   О ПРОГРАММЕ\n";
    cout << "==================================================\n";
    cout << "Программа шифрования и дешифрования текста\n\n";
    cout << "Поддерживаемые алгоритмы:\n";
    
    for (size_t i = 0; i < modules.size(); i++) {
        cout << "  " << (i + 1) << ". " << modules[i].name;
        if (!modules[i].available) cout << " [НЕ ПОДКЛЮЧЕН]";
        cout << "\n";
    }
    
    cout << "\nПоддерживаемые символы:\n";
    cout << "  - Латиница (A-Z, a-z)\n";
    cout << "  - Кириллица (А-Я, а-я)\n";
    cout << "  - Цифры (0-9)\n";
    cout << "  - Специальные символы\n\n";
    cout << "Размер алфавита: " << ALPHABET_SIZE << " символов\n";
    cout << "Версия: 4.0 (динамические модули)\n";
    cout << "==================================================\n";
    cout << "\nНажмите Enter для продолжения..."; 
    cin.get();
}

// ====== ГЛАВНАЯ ФУНКЦИЯ ======
int main() {
    initAlphabet();
    registerModules();
    
    while (true) {
        cls(); 
        showMenu();
        int c; 
        cin >> c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (c == 0) {
            cout << "\nДо свидания!\n"; 
            return 0;
        }
        
        if (c == (int)modules.size() + 1) {
            about();
            continue;
        }
        
        if (c >= 1 && c <= (int)modules.size()) {
            cipherMenu(c - 1);
        } else {
            cout << "Неверный выбор!\n";
            cout << "Нажмите Enter...";
            cin.get();
        }
    }
    
    return 0;
}
