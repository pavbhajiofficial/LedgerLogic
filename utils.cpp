#include "utils.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <ctime>
#include <algorithm>
#include <cctype>

using namespace std;

namespace Utils {

string currentDate() {
    time_t t = time(nullptr);
    tm*    tm_ptr = localtime(&t);
    char buf[16];
    strftime(buf, sizeof(buf), "%Y-%m-%d", tm_ptr);
    return string(buf);
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool isValidDate(const string& date) {
    // Expects YYYY-MM-DD
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i : {0,1,2,3,5,6,8,9}) {
        if (!isdigit((unsigned char)date[i])) return false;
    }
    int month = stoi(date.substr(5, 2));
    int day   = stoi(date.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

double readPositiveDouble(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        line = trim(line);
        try {
            size_t pos;
            double val = stod(line, &pos);
            if (pos == line.size() && val > 0.0) return val;
        } catch (...) {}
        cout << "  [!] Invalid input. Enter a positive number.\n";
    }
}

int readPositiveInt(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        line = trim(line);
        try {
            size_t pos;
            int val = stoi(line, &pos);
            if (pos == line.size() && val > 0) return val;
        } catch (...) {}
        cout << "  [!] Invalid input. Enter a positive integer.\n";
    }
}

string readNonEmptyString(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        line = trim(line);
        if (!line.empty()) return line;
        cout << "  [!] Input cannot be empty.\n";
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausePrompt() {
    cout << "\n  Press Enter to continue...";
    string dummy;
    getline(cin, dummy);
}

void printHeader(const string& title) {
    string border(title.size() + 6, '=');
    cout << "\n  " << border << "\n";
    cout << "     " << title << "\n";
    cout << "  " << border << "\n\n";
}

} // namespace Utils
