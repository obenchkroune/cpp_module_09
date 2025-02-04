#pragma once

#include <stdint.h>

#include <ctime>
#include <map>
#include <string>

#define WHITESPACE " \t\n\r\f\v"

#define FILE_ERROR    "Error: could not open file"
#define DB_ERROR      "Error: could not open database file"
#define DATABASE_PATH "./cpp_09/data.csv"

std::string ft_trim(const std::string& str);
bool        is_valid_date(int year, int month, int day, time_t& t);

class BitcoinExchange {
private:
    std::map<time_t, float> _database;

    time_t parseDate(const std::string& str) const;
    float  parseRate(const std::string& str) const;

public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    ~BitcoinExchange();

    BitcoinExchange& operator=(const BitcoinExchange& other);
    bool             processFile(const char* file_path);
    bool             loadDatabase(const char* db_path = DATABASE_PATH);
};