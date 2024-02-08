#include <iostream>
#include <cstdarg>
#include <cstdio> // For snprintf
#include <vector>
#include <cstring>
#include "utils.h"

// Function to concatenate an arbitrary number of const char* strings
const char* Utils::concatenate(int numStrings, ...) {
    // Calculate the total length of the concatenated string
    va_list args;
    va_start(args, numStrings);
    size_t totalLen = 0;
    for (int i = 0; i < numStrings; ++i) {
        const char* str = va_arg(args, const char*);
        totalLen += snprintf(nullptr, 0, "%s", str);
    }
    va_end(args);

    // Allocate memory for the concatenated string (+1 for the null terminator)
    char* result = new char[totalLen + 1];

    // Format the strings into the result buffer
    size_t pos = 0;
    va_start(args, numStrings);
    for (int i = 0; i < numStrings; ++i) {
        const char* str = va_arg(args, const char*);
        size_t len = snprintf(result + pos, totalLen + 1 - pos, "%s", str);
        pos += len;
    }
    va_end(args);

    return result;
};

// Function to split a const char* string by delimiter
std::vector<std::string> Utils::splitByDelimiter(std::string str, std::string delimiter) {
    std::vector<std::string> tokens;

    // Create a mutable copy of the input string
    char* mutableCopy = new char[str.length() + 1];
    strcpy(mutableCopy, str.c_str());

    // Tokenize the mutable copy using strtok
    char* tokenCopy = strtok(mutableCopy, delimiter.c_str());
    while (tokenCopy != nullptr) {
        // Add the token to the vector
        tokens.push_back(std::string(tokenCopy));
        // Move to the next token
        tokenCopy = strtok(nullptr, delimiter.c_str());
    }

    // Clean up the mutable copy
    delete[] mutableCopy;

    return tokens;
}

const char* Utils::copyCharArray(const char* charArray) {
    char* charTmp = new char[strlen(charArray) + 1];
    strcpy(charTmp, charArray);

    return charTmp;
};
