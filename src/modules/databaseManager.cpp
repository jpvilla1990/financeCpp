#include "utils.h"

class DatabaseManager {
public:
    DatabaseManager(std::string parentPath) {
        this->parentPath = parentPath;
    };
    
    void write() {
        while (true) {
            std::cout << "writting" << std::endl;
            Utils::delay(databaseDelay);
        }
    };
private:
    int databaseDelay = 10;
    std::string parentPath;
};