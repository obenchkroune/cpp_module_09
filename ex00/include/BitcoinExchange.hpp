#pragma once

#include <map>
#include <string>

class BitcoinExchange {
public:
    static void loadDatabase(const std::string& filename);
    static void processInput(const std::string& input);

private:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();

    static std::map<std::string, float> _database;

    static void insertDatabaseRow(const std::string& key, float value);
};
