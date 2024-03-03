// utils.h
#ifndef UTILS_H
#define UTILS_H

class Utils {
public:
	static const char* concatenate(int, ...);
	static std::vector<std::string> splitByDelimiter(std::string, std::string);
	static const char* copyCharArray(const char*);
	static std::string removePrefixFromString(std::string, std::string);
	static void saveInFile(std::string, std::string);
	static std::string loadFromFile(std::string);
	static void delay(int);
};

#endif // UTILS_H
