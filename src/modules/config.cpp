#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "utils.h"
#include "config.h"
#include "constants.h"

Config::Config(const char* parentPath) {
    api = new Api();
    this->parentPath = parentPath;
    loadConfigFromJsonFile(this->parentPath, JSON_FILE);
};
void Config::addStock(std::string value){
    std::string key = CONFIG_STOCKS_PARAM_PREFIX + value;
    this->api->rapidApi[key] = RAPIDAPI_URL + value + RAPIDAPI_QUERY;
    this->saveConfigToJsonFile(this->parentPath, JSON_FILE);
};
void Config::loadConfigFromJsonFile(const char* exePath, const char* jsonFile) {
    std::string filePath = std::string(exePath) + "/" + jsonFile;

    if (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath)) {

        std::ifstream file(filePath);
        if (file.is_open()) {
            nlohmann::json j;
            file >> j;

            // Fill the rapidApi map from JSON
            for (auto& [key, value] : j["rapidApi"].items()) {
                std::string valueString = value.get<std::string>();

                api->rapidApi[key] = valueString;
            }
            for (auto& [key, value] : j["database"].items()) {
                std::string valueString = value.get<std::string>();

                database[key] = valueString;
            }
            for (auto& [key, value] : j["server"].items()) {
                std::string valueString = value.get<std::string>();

                server[key] = valueString;
            }
            file.close();
        }
        else {
            std::cerr << "Unable to open file: " << filePath << std::endl;
            throw std::runtime_error("Unable to open file\n");
        }
    }
    else {
        std::cerr << "File not found: " << filePath << std::endl;
        throw std::runtime_error("File not found\n");
    }
};
void Config::saveConfigToJsonFile(const char* exePath, const char* jsonFile){
    std::string filePath = std::string(exePath) + "/" + jsonFile;
    if (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath)) {

        std::ifstream file(filePath);
        if (file.is_open()) {
            nlohmann::json j;
            file >> j;
            file.close();

            std::ofstream file(filePath);
            for (auto& [key, value] : api->rapidApi) {
                j["rapidApi"][key] = value;
            }
            for (auto& [key, value] : database) {
                j["database"][key] = value;
            }
            for (auto& [key, value] : server) {
                j["server"][key] = value;
            }
            file << std::setw(4) << j << std::endl;
            file.close();
        }
        else {
            std::cerr << "Unable to open file: " << filePath << std::endl;
            throw std::runtime_error("Unable to open file\n");
        }
    }
    else {
        std::cerr << "File not found: " << filePath << std::endl;
        throw std::runtime_error("File not found\n");
    }
}