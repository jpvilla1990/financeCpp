#include <iostream>
#include <cstdarg>
#include <cstdio> // For snprintf

class Utils {
public:
    // Function to concatenate an arbitrary number of const char* strings
    static const char* concatenate(int numStrings, ...) {
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
    }

    // Function to split a const char* string by delimiter
    static std::vector<const char*> splitByDelimiter(const char* str, const char* delimiter) {
        std::vector<const char*> tokens;
        char* mutableCopy = new char[strlen(str) + 1];
        strcpy(mutableCopy, str);

        char* tokenCopy = strtok(mutableCopy, delimiter);
        char* token = new char[strlen(tokenCopy) + 1];
        strcpy(token, tokenCopy);

        while (token != nullptr) {
            tokens.push_back(token);
            token = strtok(nullptr, delimiter);
        }

        delete[] mutableCopy;
        return tokens;
    }
};
