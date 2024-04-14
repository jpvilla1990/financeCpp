// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <map>

class Config {
public:
	struct Api {
        std::map<std::string, std::string> rapidApi;
	};
	
	std::map<std::string, std::string> database;
	std::map<std::string, std::string> server;
	Api* api;

	Config(const char*);
	void addStock(std::string);
private:
	const char* parentPath;
	void loadConfigFromJsonFile(const char*, const char*);
	void saveConfigToJsonFile(const char*, const char*);
};

#endif