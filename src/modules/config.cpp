#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "utils.h"
#include "config.h"

Config::Config(const char* parentPath) {
    api = new Api();
    loadConfigFromJsonFile(parentPath, "config.json");
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