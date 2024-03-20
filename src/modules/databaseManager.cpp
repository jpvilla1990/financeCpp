#include <nlohmann/json.hpp>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include "utils.h"
#include "constants.h"
#include "fileSystem.h"
#include "config.h"
#include <mysql_connection.h>

class DatabaseManager : FileSystem {
public:
    DatabaseManager(std::string parentPath) : FileSystem(parentPath) {
        const char* exePath = this->parentPath.c_str();
        Config* config = new Config(exePath);

        std::string url = config->database["url"];
        std::string user = config->database["user"];
        std::string password = config->database["password"];

        initDatabase(
            url,
            user,
            password
        );
    };
    
    void write() {
        while (true) {
            const char* exePath = this->parentPath.c_str();
            Config* config = new Config(exePath);

            for (auto it = config->api->rapidApi.begin(); it != config->api->rapidApi.end(); ++it) {
                auto& key = it->first;

                std::string stock = Utils::removePrefixFromString(key, STOCK_PREFIX);

                if (stock == "") {
                    // skip iteration on json if the key is not a st�ck
                    continue;
                }

                std::string stockData = Utils::loadFromFile(this->parentPath + FILE_SEPARATOR + DATA_FOLDER + FILE_SEPARATOR + stock + TXT_EXT);

                if (stockData == "") {
                    continue;
                }
                else {
                    std::map<std::string, double> stockDataMap = parseStockDataValues(stockData);
                    std::map<std::string, std::string> stockMetadataMap = parseStockMetadata(stockData);
                    //TODO: injest data

                    for (const auto& pair : stockDataMap) {
                        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
                    }

                    for (const auto& pair : stockMetadataMap) {
                        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
                    }
                }
            }

            Utils::delay(databaseDelay);
        }
    };

private:
    int databaseDelay = 10;
    sql::Driver *driver;
    sql::Connection *con;

    void initDatabase(std::string url, std::string user, std::string password){
        try {
            driver = get_driver_instance();
            con = driver->connect(url, user, password);
        } catch (sql::SQLException &e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    std::map<std::string, std::string> parseStockMetadata(std::string stockData) {
        std::map<std::string, std::string> stockDataMap;
        nlohmann::json j = nlohmann::json::parse(stockData);

        stockDataMap[RAPIDAPI_FINANCIALCURRENCY] = readMetaStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_FINANCIALCURRENCY
        );
        stockDataMap[RAPIDAPI_SYMBOL] = readMetaStockValuesFromJson(
            j,
            RAPIDAPI_META,
            RAPIDAPI_SYMBOL
        );
        stockDataMap[RAPIDAPI_PROCESSEDTIME] = readMetaStockValuesFromJson(
            j,
            RAPIDAPI_META,
            RAPIDAPI_PROCESSEDTIME
        );

        return stockDataMap;
    }

    std::map<std::string, double> parseStockDataValues(std::string stockData) {
        std::map<std::string, double> stockDataMap;
        nlohmann::json j = nlohmann::json::parse(stockData);

        stockDataMap[RAPIDAPI_CURRENTPRICE] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_CURRENTPRICE,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_TOTALCASH] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_TOTALCASH,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_TOTALCASHPERSHARE] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_TOTALCASHPERSHARE,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_EBITDA] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_EBITDA,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_TOTALDEBT] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_TOTALDEBT,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_QUICKRATIO] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_QUICKRATIO,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_CURRENTRATIO] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_CURRENTRATIO,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_TOTALREVENUE] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_TOTALREVENUE,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_DEBTTOEQUITY] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_DEBTTOEQUITY,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_REVENUEPERSHARE] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_REVENUEPERSHARE,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_RETURNONASSETS] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_RETURNONASSETS,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_RETURNONEQUITY] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_RETURNONEQUITY,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_GROSSPROFITS] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_GROSSPROFITS,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_FREECASHFLOW] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_FREECASHFLOW,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_OPERATINGCASHFLOW] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_OPERATINGCASHFLOW,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_EARNINGSGROWTH] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_EARNINGSGROWTH,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_REVENUEGROWTH] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_REVENUEGROWTH,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_GROSSMARGINS] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_GROSSMARGINS,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_EBITDAMARGINS] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_EBITDAMARGINS,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_OPERATINGMARGINS] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_OPERATINGMARGINS,
            RAPIDAPI_FMT
        );
        stockDataMap[RAPIDAPI_PROFITMARGINS] = readStockValuesFromJson(
            j,
            RAPIDAPI_BODY,
            RAPIDAPI_PROFITMARGINS,
            RAPIDAPI_FMT
        );

        return stockDataMap;
    };

    std::string readMetaStockValuesFromJson(nlohmann::json j, std::string key1, std::string key2) {
        std::string value = "";
        try {
            value = j[key1][key2];
        }
        catch (const std::exception& e) {
            //TODO: log exception
        }
        return value;
    };

    double readStockValuesFromJson(nlohmann::json j, std::string key1, std::string key2, std::string key3) {
        double value = 0;
        try {
            value = j[key1][key2][key3];
        }
        catch (const std::exception& e) {
            //TODO: log exception
        }
        return value;
    };
};