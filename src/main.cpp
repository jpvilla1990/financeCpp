#include <iostream>
#include <string>
#include <filesystem>
#include "config.cpp"
#include "modules/curl.cpp"

int main(int argc, char* argv[]) {
    std::string exePathStr = std::filesystem::path(argv[0]).parent_path().string();
    const char* exePath = exePathStr.c_str();
    Config config = Config(exePath);
    std::string url = config.api->rapidApi.at("url");
    std::string headers = config.api->rapidApi.at("headers");
    std::string request = config.api->rapidApi.at("stocksRequest0");

    Curl* curl = new Curl();
    std::string response = curl->sendGetRequest(url, headers, request);
    std::cout << response << std::endl;

    return 0;
}
