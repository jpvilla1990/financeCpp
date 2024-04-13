#include <string>
#include <filesystem>
#include <thread>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "modules/sourceRequestManager.cpp"
#include "modules/databaseManager.cpp"

using namespace boost::asio;
namespace http = boost::beast::http;

SourceRequestManager* sourceRequestManager;
DatabaseManager* databaseManager;
Config* config;

void runSourceRequestManager(SourceRequestManager* sourceRequestManager) {
    sourceRequestManager->run();
}

void writeDatabase(DatabaseManager* databaseManager) {
    databaseManager->write();
}

void handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    res.version(req.version());
    res.set(http::field::server, "Boost REST Server");

    std::string requestStringFull = req.target().to_string();
    std::vector<std::string> requestVector = Utils::splitByDelimiter(requestStringFull, GET_REQUEST_DELIMITER);
    std::string requestString = requestVector.at(0);

    if (req.method() == http::verb::get) {
        if (requestString == START_COLLECTION) {
            res.result(http::status::ok);
            res.body() = START_COLLECTION_ACTION;
            std::thread sourceThread(runSourceRequestManager, sourceRequestManager);
            sourceThread.detach();
        } else if(requestString == STOP_COLLECTION){
            res.result(http::status::ok);
            res.body() = STOP_COLLECTION_ACTION;
            sourceRequestManager->stop();
        } else if (requestString == START_DATABASE_INGESTION) {
            res.result(http::status::ok);
            res.body() = START_DATABASE_INGESTION_ACTION;
            std::thread databaseThread(writeDatabase, databaseManager);
            databaseThread.detach();
        } else if (requestString == STOP_DATABASE_INGESTION) {
            res.result(http::status::ok);
            res.body() = STOP_DATABASE_INGESTION_ACTION;
            databaseManager->stop();
        } else if(requestString == GET_STOCK_INFO){
            res.result(http::status::ok);
            std::map<std::string, std::string> params = Utils::getParamsGetRequest(requestVector.at(1));
            auto it = params.find(GET_STOCK_INFO_PARAM_STOCK);
            if (it != params.end()) {
                std::string responseStock = databaseManager->getStockData(params.at(GET_STOCK_INFO_PARAM_STOCK), STOCK_QUERY_LIMIT_ONE);
                res.body() = responseStock;
            } else {
                res.body() = SERVER_PARAM_NOT_FOUND;
            }
        } else if(requestString == ADD_STOCK){
            res.result(http::status::ok);
            std::map<std::string, std::string> params = Utils::getParamsGetRequest(requestVector.at(1));
            auto it = params.find(ADD_STOCK_INFO_PARAM_STOCK);
            if (it != params.end()) {
                config->addStock(params.at(ADD_STOCK_INFO_PARAM_STOCK));
                res.body() = "param " + params.at(ADD_STOCK_INFO_PARAM_STOCK) + " was added";
            } else {
                res.body() = SERVER_PARAM_NOT_FOUND;
            }
        } else {
            res.result(http::status::not_found);
            res.body() = SERVER_ACTION_NOT_FOUND;
        }
    } else {
        res.result(http::status::bad_request);
        res.body() = SERVER_BAD_REQUEST;
    }

    res.prepare_payload();
}

int main(int argc, char* argv[]) {
    std::string exePathStr = std::filesystem::path(argv[0]).parent_path().string();
    const char* exePath = exePathStr.c_str();

    sourceRequestManager = new SourceRequestManager(exePathStr);
    databaseManager = new DatabaseManager(exePathStr);

    config = new Config(exePath);
    std::string portString = config->server["port"];
    int port = std::stoi(portString);

    try {
        io_service ioservice;
        ip::tcp::acceptor acceptor(ioservice, ip::tcp::endpoint(ip::tcp::v4(), port));

        while (true) {
            ip::tcp::socket socket(ioservice);
            acceptor.accept(socket);

            boost::beast::flat_buffer buffer;
            http::request<http::string_body> req;

            boost::system::error_code ec;
            http::read(socket, buffer, req, ec);
            if (ec) {
                std::cerr << "Error reading request: " << ec.message() << std::endl;
                continue;
            }

            http::response<http::string_body> res;
            handle_request(req, res);

            http::write(socket, res, ec);
            if (ec) {
                std::cerr << "Error writing response: " << ec.message() << std::endl;
                continue;
            }

            socket.shutdown(ip::tcp::socket::shutdown_send, ec);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}