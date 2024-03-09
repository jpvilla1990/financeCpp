#include <iostream>
#include "config.h"
#include "curlModule.h"
#include "utils.h"
#include "constants.h"

class SourceRequestManager {
public:
    SourceRequestManager(std::string parentPath) {
        this->parentPath = parentPath;
    }
    void run() {
        int periodInSecondsInt = 0;
        while (true) {
            const char* exePath = this->parentPath.c_str();
            CurlModule* curlModule = new CurlModule();
            Config* config = new Config(exePath);
            std::string url = config->api->rapidApi.at(URL_KEY);;
            std::string headers = config->api->rapidApi.at(HEADERS_KEY);

            for (auto it = config->api->rapidApi.begin(); it != config->api->rapidApi.end(); ++it) {
                auto& key = it->first;
                auto& value = it->second;

                std::string stock = Utils::removePrefixFromString(key, STOCK_PREFIX);

                if (stock == "") {
                    // skip iteration on json if the key is not a stock
                    continue;
                }
                std::string response = curlModule->sendGetRequest(url, headers, value);
                Utils::saveInFile(response, this->parentPath + FILE_SEPARATOR + stock + TXT_EXT);
            }

            std::string periodInSeconds = config->api->rapidApi.at("periodInSeconds");
            periodInSecondsInt = std::stoi(periodInSeconds);
            Utils::delay(periodInSecondsInt);

            delete config;
            delete curlModule;
        }
    }
private:
    std::string parentPath;
};
