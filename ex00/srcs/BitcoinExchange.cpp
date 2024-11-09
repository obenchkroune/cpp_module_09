#include "BitcoinExchange.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

std::string to_string(float val)
{
    std::ostringstream oss;
    oss << val;
    if (oss.fail())
        throw std::runtime_error("to_string failed");
    return oss.str();
}

std::string str_trim(const std::string& str)
{
    std::string::const_iterator         it  = str.begin();
    std::string::const_reverse_iterator rit = str.rbegin();
    while (it != str.end() && isspace(*it))
        ++it;
    while (rit.base() != it && isspace(*rit))
        ++rit;
    return std::string(it, rit.base());
}

bool validate_date(const std::string& date)
{
    std::string trimmed = str_trim(date);
    if (trimmed.size() != 10)
        return false;
    int                year, month, day;
    char               sep1, sep2;
    std::istringstream iss(trimmed);
    iss >> year >> sep1 >> month >> sep2 >> day;
    if (iss.fail() || !iss.eof() || sep1 != '-' || sep2 != '-')
        return false;
    /* check if the values make sense */
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
        return false;
    switch (month)
    {
        case 2:
            if ((year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)))
                return day <= 28;
            return day <= 29;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            return day <= 30;
            break;
    }
    return true;
}

std::map<std::string, float> BitcoinExchange::_database;

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
    (void)other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    (void)other;
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::insertDatabaseRow(const std::string& date, float exchangeRate)
{
    if (validate_date(date) == false)
        throw std::runtime_error("invalid database date format: " + date);
    if (exchangeRate < 0)
        throw std::runtime_error("invalid exchange rate: " + to_string(exchangeRate));
    _database[date] = exchangeRate;
}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream ifs(filename.c_str());
    if (!ifs.is_open())
        throw std::runtime_error("failed to open file: " + filename);

    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line;
    std::string date;
    float       exchangeRate;
    while (std::getline(ifs >> std::ws, line))
    {
        std::istringstream iss(line);

        if (!std::getline(iss, date, ',') || !validate_date(date) || !(iss >> exchangeRate))
            continue;
        insertDatabaseRow(date, exchangeRate);
    }
}

void BitcoinExchange::processInput(const std::string& input)
{
    std::ifstream ifs(input.c_str());
    if (!ifs.is_open())
        throw std::runtime_error("failed to open file: " + input);

    std::string line;
    std::string date;
    float       exchangeRate;

    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (std::getline(ifs >> std::ws, line))
    {
        std::istringstream iss(line);
        if (!std::getline(iss, date, '|') || !validate_date(date))
            std::cerr << "Error: bad input => " << line << '\n';
        else if (!(iss >> exchangeRate))
            std::cerr << "Error: bad input => " << line << '\n';
        else if (exchangeRate < 0)
            std::cerr << "Error: not a positive number.\n";
        else if (exchangeRate > 1000)
            std::cerr << "Error: too large a number.\n";
        else
        {
            std::map<std::string, float>::const_iterator it = _database.upper_bound(date);

            if (it == _database.end())
                continue;

            if (it != _database.begin())
                --it;

            std::cout << it->first << " => " << it->second << " = " << (it->second * exchangeRate)
                      << '\n';
        }
    }
}
