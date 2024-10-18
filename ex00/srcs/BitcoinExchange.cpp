#include "include/BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

std::string BitcoinExchange::trimString(const std::string &str)
{
	std::string::const_iterator first = str.begin();
	std::string::const_reverse_iterator last = str.rbegin();

	while (first != str.end() && std::isspace(*first))
		first++;
	while (last != str.rend() && std::isspace(*last))
		last++;
	
	return std::string(first, last.base());
}

bool BitcoinExchange::isValidDateRange(int year, int month, int day)
{
	if (year < 1 || month < 1 || day < 1)
		return false;
	switch (month)
	{
        case 2:
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                return day <= 29;
			return day <= 28;
        case 4: case 6: case 9: case 11:
            return day <= 30;
        default:
            return day <= 31;
    }
}

BitcoinExchange::BitcoinExchange()
{
	this->parseDatabase("data.csv");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	*this = other;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
	{
		database = other.database;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::parseDatabase(const char *file)
{
	std::ifstream db_file(file);
	if (db_file.fail())
		throw std::runtime_error("could not open data.csv file");
	
	db_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while ((db_file >> std::ws) && !db_file.eof())
	{
		std::string date;
		float exchange_rate;

		std::getline(db_file, date, ',');
		if (!(db_file >> exchange_rate) || exchange_rate < 0 || !isValidDate(date))
			throw std::runtime_error("invalid csv file content");
		database[date] = exchange_rate;
	}
}

void BitcoinExchange::processInput(const char *file)
{
	std::ifstream input(file);
    if (!input)
        throw std::runtime_error(INPUT_FILE_ERR);

    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line, date;
    float value;
    while (std::getline(input >> std::ws, line))
    {
        std::istringstream iss(line);
        if (!std::getline(iss, date, '|'))
        {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }
		
		date = trimString(date);

        if (!isValidDate(date))
        {
            std::cout << "Error: bad input => " << date << std::endl;
            continue;
        }

        if (!(iss >> value))
        {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (value < 0)
        {
            std::cout << "Error: not a positive number." << std::endl;
            continue;
        }

        if (value > 1000)
        {
            std::cout << "Error: too large a number." << std::endl;
            continue;
        }

        std::map<std::string, float>::iterator it = database.upper_bound(date);
		
		if (it == database.end())
			continue;
		
        if (it != database.begin())
            --it;

        std::cout << date << " => " << value << " = " << (value * it->second) << std::endl;
    }
}

bool BitcoinExchange::isValidDate(const std::string &date)
{
	int year, month, day;
	char delim[3] = {0};
	std::istringstream iss(trimString(date));

	if (!(iss >> std::noskipws >> year >> delim[0] >> month >> delim[1] >> day))
		return false;
	if (!iss.eof() || std::string(delim) != "--" || !isValidDateRange(year, month, day))
		return false;
	return true;
}
