#ifndef CONSTANTS_H
#define CONSTANTS_H

#define STOCK_PREFIX "stock_"
#define FILE_SEPARATOR "/"
#define TXT_EXT ".txt"
#define DATA_FOLDER "data"
#define LOG_FOLDER "logs"
#define LOG_FILE "backEndLog.txt"

#define URL_KEY "url"
#define HEADERS_KEY "headers"

#define RAPIDAPI_BODY "body"
#define RAPIDAPI_META "meta"
#define RAPIDAPI_FMT "raw"

#define RAPIDAPI_CURRENTPRICE "currentPrice"
#define RAPIDAPI_TOTALCASH "totalCash"
#define RAPIDAPI_TOTALCASHPERSHARE "totalCashPerShare"
#define RAPIDAPI_EBITDA "ebitda"
#define RAPIDAPI_TOTALDEBT "totalDebt"
#define RAPIDAPI_QUICKRATIO "quickRatio"
#define RAPIDAPI_CURRENTRATIO "currentRatio"
#define RAPIDAPI_TOTALREVENUE "totalRevenue"
#define RAPIDAPI_DEBTTOEQUITY "debtToEquity"
#define RAPIDAPI_REVENUEPERSHARE "revenuePerShare"
#define RAPIDAPI_RETURNONASSETS "returnOnAssets"
#define RAPIDAPI_RETURNONEQUITY "returnOnEquity"
#define RAPIDAPI_GROSSPROFITS "grossProfits"
#define RAPIDAPI_FREECASHFLOW "freeCashflow"
#define RAPIDAPI_OPERATINGCASHFLOW "operatingCashflow"
#define RAPIDAPI_EARNINGSGROWTH "earningsGrowth"
#define RAPIDAPI_REVENUEGROWTH "revenueGrowth"
#define RAPIDAPI_GROSSMARGINS "grossMargins"
#define RAPIDAPI_EBITDAMARGINS "ebitdaMargins"
#define RAPIDAPI_OPERATINGMARGINS "operatingMargins"
#define RAPIDAPI_PROFITMARGINS "profitMargins"

#define RAPIDAPI_FINANCIALCURRENCY "financialCurrency"
#define RAPIDAPI_SYMBOL "symbol"
#define RAPIDAPI_PROCESSEDTIME "processedTime"

#define DATABASE_NAME "finances"
#define TABLE_STOCKS_NAME "stocks"
#define TABLE_STOCKS_SCHEMA \
    "(id INT NOT NULL AUTO_INCREMENT, " \
    "currentPrice VARCHAR(100), " \
    "totalCash VARCHAR(100), " \
    "totalCashPerShare VARCHAR(100), " \
    "ebitda VARCHAR(100), " \
    "totalDebt VARCHAR(100), " \
    "quickRatio VARCHAR(100), " \
    "currentRatio VARCHAR(100), " \
    "totalRevenue VARCHAR(100), " \
    "debtToEquity VARCHAR(100), " \
    "revenuePerShare VARCHAR(100), " \
    "returnOnAssets VARCHAR(100), " \
    "returnOnEquity VARCHAR(100), " \
    "grossProfits VARCHAR(100), " \
    "freeCashflow VARCHAR(100), " \
    "operatingCashflow VARCHAR(100), " \
    "earningsGrowth VARCHAR(100), " \
    "revenueGrowth VARCHAR(100), " \
    "grossMargins VARCHAR(100), " \
    "ebitdaMargins VARCHAR(100), " \
    "operatingMargins VARCHAR(100), " \
    "profitMargins VARCHAR(100), " \
    "financialCurrency VARCHAR(100), " \
    "symbol VARCHAR(100), " \
    "processedTime VARCHAR(100), " \
    "PRIMARY KEY (id))"

#define START_COLLECTION "/startCollection"
#define STOP_COLLECTION "/stopCollection"
#define START_DATABASE_INGESTION "/startDatabaseIngestion"
#define STOP_DATABASE_INGESTION "/stopDatabaseIngestion"

#define START_COLLECTION_ACTION "startCollectionData"
#define STOP_COLLECTION_ACTION "stopCollectionData"
#define START_DATABASE_INGESTION_ACTION "startDatabaseIngestion"
#define STOP_DATABASE_INGESTION_ACTION "stopDatabaseIngestion"
#define SERVER_ACTION_NOT_FOUND "serverActionNotFound"

#define SERVER_BAD_REQUEST "Bad Request"

#endif