#include <string>
#include <filesystem>
#include "modules/sourceRequestManager.cpp"

int main(int argc, char* argv[]) {
    std::string exePathStr = std::filesystem::path(argv[0]).parent_path().string();

    SourceRequestManager* sourceRequestManager = new SourceRequestManager(exePathStr);

    sourceRequestManager->run();

    return 0;
}