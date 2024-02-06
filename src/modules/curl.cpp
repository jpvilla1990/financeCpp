#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "utils.cpp"

class Curl {
public:
    Curl(const char* url, const char* headers) {
        initCurl();
        initSsl();
    }

    std::string sendGetRequest(const char* url, const char* headersString, const char* params) {
        const char* request = Utils::concatenate(3, url, "/", params);
        struct curl_slist* headers = setupHeaders(headersString);

        setupCurl(request, headers);

        // Set the custom write callback function
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            throw std::runtime_error("curl_easy_perform() failed\n");
        }

        curlCleanUp(headers);

        return response;
    }

private:
    const char* url;
    const char* headers;

    CURL* curl;
    CURLcode res;

    void initCurl() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (!curl) {
            std::cerr << "Failed to initialize libcurl" << std::endl;
            throw std::runtime_error("Failed to initialize libcurl\n");
        }
    }

    void initSsl() {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    curl_slist* setupHeaders(const char* headersString) {
        std::vector<const char*> headersVector = Utils::splitByDelimiter(headersString, "\r\n");
        struct curl_slist* headers = nullptr;

        for (const char* header : headersVector) {
            headers = curl_slist_append(headers, header);
        }

        return headers;
    }

    void setupCurl(const char* url, curl_slist* headers) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    void curlCleanUp(curl_slist* headers) {
        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        // Cleanup libcurl
        curl_global_cleanup();
    }

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t total_size = size * nmemb;
        output->append(static_cast<char*>(contents), total_size);

        return total_size;
    }
};