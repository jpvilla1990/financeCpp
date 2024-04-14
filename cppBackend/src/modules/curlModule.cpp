#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "curlModule.h"
#include "utils.h"


CurlModule::CurlModule() {
    initCurl();
    initSsl();
}

std::string CurlModule::sendGetRequest(std::string url, std::string headersString, std::string params) {
    std::string request = url + "/" + params;

    struct curl_slist* headers = setupHeaders(headersString);

    setupCurl(request, headers);

    // Set the custom write callback function
    std::string response;
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &response);

    // Perform the request
    res = curl_easy_perform(this->curl);

    // Check for errors
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        throw std::runtime_error("curl_easy_perform() failed\n");
    }

    curlCleanUp(headers);

    return response;
}

void CurlModule::initCurl() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    this->curl = curl_easy_init();

    if (!this->curl) {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        throw std::runtime_error("Failed to initialize libcurl\n");
    }
}

void CurlModule::initSsl() {
    curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYHOST, 0L);
}

curl_slist* CurlModule::setupHeaders(std::string headersString) {
    std::vector<std::string> headersVector = Utils::splitByDelimiter(headersString, "\r\n");
    struct curl_slist* headers = nullptr;

    for (std::string header : headersVector) {
        headers = curl_slist_append(headers, header.c_str());
    }

    return headers;
}

void CurlModule::setupCurl(std::string url, curl_slist* headers) {
    curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);
}

void CurlModule::curlCleanUp(curl_slist* headers) {
    // Clean up
    curl_easy_cleanup(this->curl);
    curl_slist_free_all(headers);

    // Cleanup libcurl
    curl_global_cleanup();
}

size_t CurlModule::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);

    return total_size;
}
