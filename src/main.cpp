#include <string>
#include <filesystem>
#include <thread>
#include "modules/sourceRequestManager.cpp"
#include "modules/databaseManager.cpp"

void runSourceRequestManager(SourceRequestManager* sourceRequestManager) {
    sourceRequestManager->run();
}

void writeDatabase(DatabaseManager* databaseManager) {
    databaseManager->write();
}

int main(int argc, char* argv[]) {
    std::string exePathStr = std::filesystem::path(argv[0]).parent_path().string();

    SourceRequestManager* sourceRequestManager = new SourceRequestManager(exePathStr);
    DatabaseManager* databaseManager = new DatabaseManager(exePathStr);

    std::thread sourceThread(runSourceRequestManager, sourceRequestManager);
    std::thread databaseThread(writeDatabase, databaseManager);

    sourceThread.join();
    databaseThread.join();

    // Cleanup
    delete sourceRequestManager;
    delete databaseManager;

    return 0;
}