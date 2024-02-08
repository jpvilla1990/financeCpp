// utils.h
#ifndef UTILS_H
#define UTILS_H

class Utils {
public:
	static const char* concatenate(int, ...);
	static std::vector<std::string> splitByDelimiter(std::string, std::string);
	static const char* copyCharArray(const char*);
};

#endif // UTILS_H
