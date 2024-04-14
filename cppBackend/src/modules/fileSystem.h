// fileSystem.h
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <chrono>

class FileSystem {
public:
	FileSystem(std::string);
	void writeLog(std::string);
	void writeError(std::string);
private:
	void createFolders();
protected:
    std::string parentPath;

	std::string getCurrentTime();
};

#endif