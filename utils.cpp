#include "utils.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <ctime>
#include <algorithm>
#include <cctype>

namespace Utils {

std::string currentDate() {
    std::time_t t = std::time(nullptr);
    std::tm*    tm_ptr = std::localtime(&t);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm_ptr);
    return std::string(buf);
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool isValidDate(const std::string& date) {
    // Expects YYYY-MM-DD
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i : {0,1,2,3,5,6,8,9}) {
        if (!std::isdigit((unsigned char)date[i])) return false;
    }
    int month = std::stoi(date.substr(5, 2));
    int day   = std::stoi(date.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

double readPositiveDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        line = trim(line);
        try {
            size_t pos;
            double val = std::stod(line, &pos);
            if (pos == line.size() && val > 0.0) return val;
        } catch (...) {}
        std::cout << "  [!] Invalid input. Enter a positive number.\n";
    }
}

int readPositiveInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        line = trim(line);
        try {
            size_t pos;
            int val = std::stoi(line, &pos);
            if (pos == line.size() && val > 0) return val;
        } catch (...) {}
        std::cout << "  [!] Invalid input. Enter a positive integer.\n";
    }
}

std::string readNonEmptyString(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        line = trim(line);
        if (!line.empty()) return line;
        std::cout << "  [!] Input cannot be empty.\n";
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
    std::cout << "\n  Press Enter to continue...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

void printHeader(const std::string& title) {
    std::string border(title.size() + 6, '=');
    std::cout << "\n  " << border << "\n";
    std::cout << "     " << title << "\n";
    std::cout << "  " << border << "\n\n";
}

} // namespace Utils
