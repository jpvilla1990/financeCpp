#include <iostream>
#include <string>
#include <filesystem>
#include "config.cpp"
#include "modules/sourceRequestManager.cpp"

int main(int argc, char* argv[]) {
    std::string exePathStr = std::filesystem::path(argv[0]).parent_path().string();
    const char* exePath = exePathStr.c_str();
    Config config = Config(exePath);
    std::string url = config.api->rapidApi.at("url");
    std::string headers = config.api->rapidApi.at("headers");
    std::string request = config.api->rapidApi.at("stocksRequest0");
    std::string periodInSeconds = config.api->rapidApi.at("periodInSeconds");

    SourceRequestManager* sourceRequestManager = new SourceRequestManager(exePathStr, url, headers, request, periodInSeconds);

    sourceRequestManager->run();

    return 0;
}