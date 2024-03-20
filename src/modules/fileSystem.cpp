#include "fileSystem.h"
#include "utils.h"
#include "constants.h"

FileSystem::FileSystem(std::string parentPath) {
    this->parentPath = parentPath;
    createFolders();
}

void FileSystem::createFolders(){
    Utils::createFolder(this->parentPath + FILE_SEPARATOR + DATA_FOLDER);
}