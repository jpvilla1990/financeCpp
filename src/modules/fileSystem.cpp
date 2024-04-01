#include "fileSystem.h"
#include "utils.h"
#include "constants.h"

FileSystem::FileSystem(std::string parentPath) {
    this->parentPath = parentPath;
    createFolders();
}

void FileSystem::createFolders(){
    Utils::createFolder(this->parentPath + FILE_SEPARATOR + DATA_FOLDER);
    Utils::createFolder(this->parentPath + FILE_SEPARATOR + LOG_FOLDER);
}

std::string FileSystem::getCurrentTime(){
    auto now = std::chrono::system_clock::now();

    // Convert to time_t (time since epoch)
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert time_t to struct tm (broken down time)
    std::tm *timeInfo = std::localtime(&currentTime);

    // Format the time into a string
    char buffer[80];
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeInfo);

    return std::string(buffer);
}

void FileSystem::writeLog(std::string log){
    std::string content = "TIMESTAMP: " + FileSystem::getCurrentTime() + " LOG_BACK_END: " + log + "\n";
    Utils::appendInFile(content, this->parentPath + FILE_SEPARATOR  + LOG_FOLDER + FILE_SEPARATOR + LOG_FILE);
}