#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "curl.cpp"

class SourceRequestManager {
public:
    SourceRequestManager(std::string parentPath, std::string url, std::string headers, std::string request, std::string periodInSeconds) {
        this->url = url;
        this->headers = headers;
        this->request = request;
        this->periodInSeconds = periodInSeconds;
        this->parentPath = parentPath;
        this->curl = new Curl();
    };
    void run() {
        //TODO: request and stock name will be update to be read from database
        std::string response = curl->sendGetRequest(this->url, this->headers, this->request);
        saveInFile(response, this->parentPath + "/stock.txt");
    }
private:
    std::string url;
    std::string headers;
    std::string request;
    std::string periodInSeconds;
    std::string parentPath;
    Curl* curl;

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
