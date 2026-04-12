#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    std::string currentDate();          // Returns "YYYY-MM-DD"
    std::string trim(const std::string& s);
    bool        isValidDate(const std::string& date);
    double      readPositiveDouble(const std::string& prompt);
    int         readPositiveInt(const std::string& prompt);
    std::string readNonEmptyString(const std::string& prompt);
    void        clearScreen();
    void        pausePrompt();
    void        printHeader(const std::string& title);
}

#endif
