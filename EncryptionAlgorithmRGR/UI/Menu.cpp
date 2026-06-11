#include "Menu.h"
#include "../FileHandler/FileHandler.h"
#include "../KeyGen/KeyGen.h"
#include <iostream>
#include <limits>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
    #define LOAD_LIB(p) LoadLibraryA(p)
    #define GET_FUNC(h, n) GetProcAddress((HMODULE)h, n)
    #define CLOSE_LIB(h) FreeLibrary((HMODULE)h)
    #define LIB_EXT ".dll"
#else
    #include <dlfcn.h>
    #define LOAD_LIB(p) dlopen(p, RTLD_LAZY)
    #define GET_FUNC(h, n) dlsym(h, n)
    #define CLOSE_LIB(h) dlclose(h)
    #define LIB_EXT ".so"
#endif

void Menu::show() {
    while (true) {
        cout << "\n  __  __ ______ _   _ _   _ " << endl;
        cout << " |  \\/  |  ____| \\ | | | | |" << endl;
        cout << " | \\  / | |__  |  \\| | | | |" << endl;
        cout << " | |\\/| |  __| | . ` | | | |" << endl;
        cout << " | |  | | |____| |\\  | |_| |" << endl;
        cout << " |_|  |_|______|_| \\_|\\___/" << endl;
        cout << endl;
        cout << "  1. Шифровка/дешифровка текста" << endl;
        cout << "  2. Шифровка/дешифровка файла" << endl;
        cout << "  3. Генератор ключей" << endl;
        cout << "  4. Выход" << endl;
        cout << "  > ";
        int ch;
        cin >> ch;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        try {
            switch (ch) {
                case 1: doText(); break;
                case 2: doFile(); break;
                case 3: doKey(); break;
                case 4:
                    cout << "\n   ____             " << endl;
                    cout << "  |  _ \\            " << endl;
                    cout << "  | |_) |_   _  ___ " << endl;
                    cout << "  |  _ <| | | |/ _ \\" << endl;
                    cout << "  | |_) | |_| |  __/" << endl;
                    cout << "  |____/ \\__, |\\___|" << endl;
                    cout << "          __/ |     " << endl;
                    cout << "         |___/      " << endl;
                    cout << "\n  Пока-пока!" << endl;
                    return;
                default: cout << "  [X] Неверный выбор." << endl;
            }
        } catch (const exception& e) {
            cerr << "  [X] Ошибка: " << e.what() << endl;
            pause();
        }
    }
}

string Menu::ask(const string& msg) {
    cout << "  " << msg;
    string in;
    getline(cin, in);
    return in;
}

ICipher* Menu::pickCiph() {
    cout << "\n   _____ _ _           " << endl;
    cout << "  / ____(_) |          " << endl;
    cout << " | |     _| |__  _ __  " << endl;
    cout << " | |    | | '_ \\| '_ \\ " << endl;
    cout << " | |____| | |_) | |_) |" << endl;
    cout << "  \\_____|_| .__/| .__/" << endl;
    cout << "          | |   | |    " << endl;
    cout << "          |_|   |_|    " << endl;
    cout << endl;
    cout << "  1. XOR-шифр" << endl;
    cout << "  2. Перестановка" << endl;
    cout << "  > ";
    int ch;
    cin >> ch;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string lib;
    if (ch == 1) lib = "./Plugins/Xor/libxorciph" LIB_EXT;
    else if (ch == 2) lib = "./Plugins/Transp/libtransp" LIB_EXT;
    else throw runtime_error("Неизвестный алгоритм");
    void* h = LOAD_LIB(lib.c_str());
    if (!h) throw runtime_error("Не вышло загрузить: " + lib);
    MakeCipher mc = (MakeCipher)GET_FUNC(h, "createCipher");
    if (!mc) { CLOSE_LIB(h); throw runtime_error("Нет createCipher в либе"); }
    return mc();
}

void Menu::doText() {
    cout << "\n  _______        _   " << endl;
    cout << " |__   __|      | |  " << endl;
    cout << "    | | _____  _| |_ " << endl;
    cout << "    | |/ _ \\ \\/ / __|" << endl;
    cout << "    | |  __/>  <| |_ " << endl;
    cout << "    |_|\\___/_/\\_\\\\__|" << endl;
    cout << endl;
    ICipher* cph = pickCiph();
    cout << "  [1] Зашифровать  [2] Расшифровать: ";
    int op;
    cin >> op;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string key = ask("Ключ: ");
    if (!cph->keyOk(key)) { cout << "  [X] Ключ не подходит!" << endl; delete cph; return; }
    string txt = ask("Текст: ");
    string out;
    if (op == 1) out = cph->encode(txt, key);
    else if (op == 2) out = cph->decode(txt, key);
    else { cout << "  [X] Не то." << endl; delete cph; return; }
    cout << "\n  +-------------------+" << endl;
    cout << "  | Итог: " << out << endl;
    cout << "  +-------------------+" << endl;
    delete cph;
    pause();
}

void Menu::doFile() {
    cout << "\n  ______ _ _      " << endl;
    cout << " |  ____(_) |     " << endl;
    cout << " | |__   _| | ___ " << endl;
    cout << " |  __| | | |/ _ \\" << endl;
    cout << " | |    | | |  __/" << endl;
    cout << " |_|    |_|_|\\___|" << endl;
    cout << endl;
    ICipher* cph = pickCiph();
    cout << "  [1] Зашифровать  [2] Расшифровать: ";
    int op;
    cin >> op;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string key = ask("Ключ: ");
    if (!cph->keyOk(key)) { cout << "  [X] Ключ не подходит!" << endl; delete cph; return; }
    string inp = ask("Файл откуда: ");
    if (!FH::exists(inp)) { cout << "  [X] Файл не найден!" << endl; delete cph; return; }
    string outp = ask("Файл куда: ");
    FH::mkdirs(outp);
    cout << "  [1] Текст  [2] Бинарь: ";
    int ft;
    cin >> ft;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    try {
        if (ft == 1) {
            string data = FH::readTxt(inp);
            string res = (op == 1) ? cph->encode(data, key) : cph->decode(data, key);
            FH::writeTxt(outp, res);
        } else {
            auto data = FH::readBin(inp);
            auto res = (op == 1) ? cph->encodeBin(data, key) : cph->decodeBin(data, key);
            FH::writeBin(outp, res);
        }
        cout << "\n  +-----------------------------+" << endl;
        cout << "  | [+] Сохранено: " << outp << endl;
        cout << "  +-----------------------------+" << endl;
    } catch (const exception& e) {
        cerr << "  [X] Ошибка файла: " << e.what() << endl;
    }
    delete cph;
    pause();
}

void Menu::doKey() {
    cout << "\n  _  ________   ______ _   _ " << endl;
    cout << " | |/ /  ____| |  ____| \\ | |" << endl;
    cout << " | ' /| |__    | |__  |  \\| |" << endl;
    cout << " |  < |  __|   |  __| | . ` |" << endl;
    cout << " | . \\| |____  | |____| |\\  |" << endl;
    cout << " |_|\\_\\______| |______|_| \\_|" << endl;
    cout << endl;
    ICipher* cph = pickCiph();
    string k = KG::makeKey(cph);
    cout << "\n  +-------------------------+" << endl;
    cout << "  | " << cph->name() << " ключ: " << k << endl;
    cout << "  +-------------------------+" << endl;
    delete cph;
    pause();
}

void Menu::pause() {
    cout << "\n  [Enter] чтоб дальше...";
    cin.get();
}
