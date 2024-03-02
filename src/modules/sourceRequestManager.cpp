#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "config.h"
#include "curlModule.h"
#include "utils.h"

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
            std::string url = config->api->rapidApi.at("url");;
            std::string headers = config->api->rapidApi.at("headers");

            for (auto it = config->api->rapidApi.begin(); it != config->api->rapidApi.end(); ++it) {
                auto& key = it->first;
                auto& value = it->second;

                std::string stock = Utils::removePrefixFromString(key, "stock_");

                if (stock == "") {
                    // skip iteration on json if the key is not a stóck
                    continue;
                }

                //TODO: request and stock name will be update to be read from database
                std::string response = curlModule->sendGetRequest(url, headers, value);
                saveInFile(response, this->parentPath + "/" + stock + ".txt");
            }

            delete config;
            delete curlModule;

            std::string periodInSeconds = config->api->rapidApi.at("periodInSeconds");
            periodInSecondsInt = std::stoi(periodInSeconds);
            Utils::delay(periodInSecondsInt);
        }
    }
private:
    std::string parentPath;

    void saveInFile(std::string content, std::string fileName) {
        std::ofstream outputFile(fileName);

        // Check if the file is opened successfully
        if (outputFile.is_open()) {
            // Write the string content to the file
            outputFile << content;

            // Close the file
            outputFile.close();

            std::cout << "Content has been saved to " << fileName << std::endl;
        }
        else {
            // If the file could not be opened, print an error message
            std::cerr << "Unable to open the file for writing." << std::endl;
            throw std::runtime_error("Unable to open the file for writing\n");
        }
    }
};
