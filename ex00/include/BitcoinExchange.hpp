#pragma once

#include <map>
#include <string>

#define INPUT_FILE_ERR "Error: could not open file."

class BitcoinExchange
{
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void processInput(const char *file);
private:
	std::map<std::string, float> database;

	void parseDatabase(const char *file);
	bool isValidDateRange(int year, int month, int day);
	std::string trimString(const std::string &str);
	bool isValidDate(const std::string &date);
};
