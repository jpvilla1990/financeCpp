#include <nlohmann/json.hpp>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include "utils.h"
#include "constants.h"
#include "fileSystem.h"
#include "config.h"
#include <string>
#include <iostream>

class DatabaseManager : FileSystem {
public:
    DatabaseManager(std::string parentPath) : FileSystem(parentPath) {
        const char* exePath = this->parentPath.c_str();
        Config* config = new Config(exePath);

        std::string url = config->database["url"];
        std::string user = config->database["user"];
        std::string password = config->database["password"];
        this->databaseName = DATABASE_NAME;
        this->tableStocksName = TABLE_STOCKS_NAME;
        this->tableStocksSchema = TABLE_STOCKS_SCHEMA;

        initDatabase(
            url,
            user,
            password
        );
    };
    
    void write() {
        if(this->running) {
            return;
        } else {
            this->running = true;
        }
        while (this->running) {
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
                    insertData(stockMetadataMap, stockDataMap);
                }
            }

            Utils::delay(databaseDelay);
        }
    };

    void stop() {
        this->running = false;
    }

    std::string getStockData(std::string stockName, int limit){
        std::string queryResponse = "{\"" + stockName + "\" : {";
        std::string limitString = std::to_string(limit);
        std::string sqlCommand = "SELECT * FROM " + this->tableStocksName + " WHERE " + RAPIDAPI_SYMBOL + " = '" + stockName + "' ORDER BY id DESC LIMIT " + limitString;
        try {
            sql::ResultSet *res;

            res = this->stmt->executeQuery(sqlCommand);
            writeLog(sqlCommand);

            // Process the result
            if (res->next()) {
                queryResponse += "\"" + std::string(RAPIDAPI_CURRENTPRICE) + "\" : \"" + res->getString(RAPIDAPI_CURRENTPRICE) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_TOTALCASH) + "\" : \"" + res->getString(RAPIDAPI_TOTALCASH) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_TOTALCASHPERSHARE) + "\" : \"" + res->getString(RAPIDAPI_TOTALCASHPERSHARE) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_EBITDA) + "\" : \"" + res->getString(RAPIDAPI_EBITDA) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_TOTALDEBT) + "\" : \"" + res->getString(RAPIDAPI_TOTALDEBT) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_QUICKRATIO) + "\" : \"" + res->getString(RAPIDAPI_QUICKRATIO) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_CURRENTRATIO) + "\" : \"" + res->getString(RAPIDAPI_CURRENTRATIO) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_TOTALREVENUE) + "\" : \"" + res->getString(RAPIDAPI_TOTALREVENUE) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_DEBTTOEQUITY) + "\" : \"" + res->getString(RAPIDAPI_DEBTTOEQUITY) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_REVENUEPERSHARE) + "\" : \"" + res->getString(RAPIDAPI_REVENUEPERSHARE) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_RETURNONASSETS) + "\" : \"" + res->getString(RAPIDAPI_RETURNONASSETS) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_RETURNONEQUITY) + "\" : \"" + res->getString(RAPIDAPI_RETURNONEQUITY) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_GROSSPROFITS) + "\" : \"" + res->getString(RAPIDAPI_GROSSPROFITS) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_FREECASHFLOW) + "\" : \"" + res->getString(RAPIDAPI_FREECASHFLOW) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_OPERATINGCASHFLOW) + "\" : \"" + res->getString(RAPIDAPI_OPERATINGCASHFLOW) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_EARNINGSGROWTH) + "\" : \"" + res->getString(RAPIDAPI_EARNINGSGROWTH) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_REVENUEGROWTH) + "\" : \"" + res->getString(RAPIDAPI_REVENUEGROWTH) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_GROSSMARGINS) + "\" : \"" + res->getString(RAPIDAPI_GROSSMARGINS) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_EBITDAMARGINS) + "\" : \"" + res->getString(RAPIDAPI_EBITDAMARGINS) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_OPERATINGMARGINS) + "\" : \"" + res->getString(RAPIDAPI_OPERATINGMARGINS) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_PROFITMARGINS) + "\" : \"" + res->getString(RAPIDAPI_PROFITMARGINS) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_FINANCIALCURRENCY) + "\" : \"" + res->getString(RAPIDAPI_FINANCIALCURRENCY) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_SYMBOL) + "\" : \"" + res->getString(RAPIDAPI_SYMBOL) + "\", ";
                queryResponse += "\"" + std::string(RAPIDAPI_PROCESSEDTIME) + "\" : \"" + res->getString(RAPIDAPI_PROCESSEDTIME) + "\"";
            } else {
                writeLog("No entries found for symbol " + stockName);
            }

            delete res;
        } catch (sql::SQLException &e) {
            writeLog("SQL get Data " + (std::string)e.what());
            std::cout << e.what() << std::endl;
        } catch (std::exception &e) {
            writeLog("Error " + (std::string)e.what());
            std::cout << e.what() << std::endl;
        }

        queryResponse += "}}";

        return queryResponse;
    }

private:
    bool running = false;
    int databaseDelay = 10;
    sql::Driver *driver;
    sql::Connection *con;
    std::unique_ptr<sql::Statement> stmt;
    std::string databaseName;
    std::string tableStocksName;
    std::string tableStocksSchema;

    void initDatabase(std::string url, std::string user, std::string password){
        try {
            driver = get_driver_instance();

            const int maxRetries = 10;
            int retryCount = 0;
            while (retryCount < maxRetries) {
                try {
                    driver = get_driver_instance();
                    con = driver->connect(url, user, password);
                    if (con) {
                        writeLog("Successfully connected to the database.");
                        break;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Failed to connect to the database: " << e.what() << std::endl;
                    writeLog("Failed to connect to the database:" + (std::string)e.what());
                }
                // Wait for a while before retrying
                Utils::delay(databaseDelay);
                retryCount++;
            }

            if (!con) {
                throw std::runtime_error("Failed to connect to the database after maximum retry attempts.");
                writeLog("Failed to connect to the database after maximum retry attempts.");
            }

            this->stmt.reset(con->createStatement());
            this->stmt->execute("CREATE DATABASE IF NOT EXISTS " + this->databaseName);
            con->setSchema(this->databaseName);
            this->stmt->execute("CREATE TABLE IF NOT EXISTS " + this->tableStocksName + " " + this->tableStocksSchema + "");
            writeLog("SQL create table " + this->tableStocksName);
        } catch (sql::SQLException &e) {
            writeLog("SQL create table error " + (std::string)e.what());
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    void insertData(std::map<std::string, std::string> stockMetadataMap, std::map<std::string, double> stockDataMap){
        // Insert some data into the table in the following format:
        // INSERT INTO your_table_name (column1, column2, column3)
        // VALUES
        //     (value1_row1, value2_row1, value3_row1),
        try {
            std::string sqlCommandPrefix = "INSERT INTO " + this->tableStocksName + " (";
            std::string sqlCommandRowNames = "";
            std::string sqlCommandValues = ") \nVALUES\n(";
            std::string sqlCommandEnd = ");";

            bool firstElement = true;
            for(auto it = stockMetadataMap.begin(); it != stockMetadataMap.end(); ++it) {
                if(firstElement){
                    firstElement = false;
                    sqlCommandRowNames += it->first;
                    sqlCommandValues += "'" + it->second + "'";
                } else {
                    sqlCommandRowNames += ", " + it->first;
                    sqlCommandValues += ", '" + it->second + "'";
                }
            }

            for(auto it = stockDataMap.begin(); it != stockDataMap.end(); ++it) {
                sqlCommandRowNames += ", " + it->first;
                sqlCommandValues += ", '" + std::to_string(it->second) + "'";
            }

            std::string sqlCommand = sqlCommandPrefix + sqlCommandRowNames + sqlCommandValues + sqlCommandEnd;
            this->stmt->execute(sqlCommand);

            writeLog("SQL command " + sqlCommand);

            std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM " + this->tableStocksName));
        } catch (sql::SQLException &e) {
            writeLog("SQL command error " + (std::string)e.what());
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
            writeLog("Stocks read from json error " + key1 + " " + key2);
        }
        return value;
    };

    double readStockValuesFromJson(nlohmann::json j, std::string key1, std::string key2, std::string key3) {
        double value = 0;
        try {
            value = j[key1][key2][key3];
        }
        catch (const std::exception& e) {
            writeLog("Stocks read from json error " + key1 + " " + key2 + " " + key3);
        }
        return value;
    };
};