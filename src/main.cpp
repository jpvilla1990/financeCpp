#include <iostream>
#include <string>
#include "config.cpp"
#include "modules/curl.cpp"

int main() {
    Config config = Config();
    const char* url = config.api->rapidApi.at("url");
    const char* headers = config.api->rapidApi.at("headers");
    const char* request = config.api->rapidApi.at("stocksRequest0");

    Curl* curl = new Curl(url, headers);
    std::string response = curl->sendGetRequest(url, headers, request);
    std::cout << response << std::endl;

    return 0;
}
