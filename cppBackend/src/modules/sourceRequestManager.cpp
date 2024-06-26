#include <iostream>
#include "curlModule.h"
#include "utils.h"
#include "constants.h"
#include "fileSystem.h"
#include "config.h"

class SourceRequestManager : FileSystem {
public:
    SourceRequestManager(std::string parentPath) : FileSystem(parentPath) {
    }

    void run() {
        if(this->running) {
            return;
        } else {
            this->running = true;
        }
        int periodInSecondsInt = 0;
        while (this->running) {
            const char* exePath = this->parentPath.c_str();
            Config* config = new Config(exePath);
            std::string url = config->api->rapidApi.at(URL_KEY);;
            std::string headers = config->api->rapidApi.at(HEADERS_KEY);

            for (auto it = config->api->rapidApi.begin(); it != config->api->rapidApi.end(); ++it) {
                CurlModule* curlModule = new CurlModule();
                auto& key = it->first;
                auto& value = it->second;

                std::string stock = Utils::removePrefixFromString(key, STOCK_PREFIX);

                if (stock == "") {
                    // skip iteration on json if the key is not a stock
                    continue;
                }
                writeLog("request " + url + " " + headers + " " + value);
                try {
                    std::string response = curlModule->sendGetRequest(url, headers, value);
                    Utils::saveInFile(response, this->parentPath + FILE_SEPARATOR  + DATA_FOLDER + FILE_SEPARATOR + stock + TXT_EXT);    
                } catch (std::exception &e) {
                    writeError("Error " + (std::string)e.what());
                    std::cout << e.what() << std::endl;
                }

                delete curlModule;
            }

            std::string periodInSeconds = config->api->rapidApi.at("periodInSeconds");
            periodInSecondsInt = std::stoi(periodInSeconds);

            Utils::delay(periodInSecondsInt);

            delete config;
        }
    }

    void stop() {
        this->running = false;
    }

private:
    bool running = false;
};
