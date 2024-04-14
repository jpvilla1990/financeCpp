#ifndef CURLMODULE_H
#define CURLMODULE_H

#include <vector>
#include <curl/curl.h>

class CurlModule {
public:
    CurlModule();
    std::string sendGetRequest(std::string, std::string, std::string);

private:
    CURL* curl;
    CURLcode res;

    void initCurl();

    void initSsl();

    curl_slist* setupHeaders(std::string);

    void setupCurl(std::string, curl_slist*);

    void curlCleanUp(curl_slist*);

    static size_t WriteCallback(void*, size_t, size_t, std::string*);
};

#endif