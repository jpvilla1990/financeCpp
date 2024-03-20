// fileSystem.h
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

class FileSystem {
public:
	FileSystem(std::string);
private:
	void createFolders();
protected:
    std::string parentPath;
};

#endif