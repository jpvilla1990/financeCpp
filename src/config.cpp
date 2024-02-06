#include <map>

class Config {
public:
	struct Api {
		std::map<const char*, const char*> rapidApi = {
			{"url", "https://yahoo-finance15.p.rapidapi.com"},
			{"headers", "X-RapidAPI-Host: yahoo-finance15.p.rapidapi.com\r\nX-RapidAPI-Key: e12e13356cmsh23c92219c4f47d8p127226jsnb98aeeb4c7cf"},
			{"stocksRequest0", "api/v1/markets/stock/modules?ticker=PAH3.DE&module=financial-data"},
		};
	};
	const Api* api;

	Config() {
		api = new Api();
	};
};