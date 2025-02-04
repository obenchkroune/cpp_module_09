#include "BitcoinExchange.hpp"

#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>

std::string ft_trim(const std::string& str) {
    std::string::size_type begin = str.find_first_not_of(WHITESPACE);
    if (begin == std::string::npos) return "";
    std::string::size_type end = str.find_last_not_of(WHITESPACE);

    return str.substr(begin, end - begin + 1);
}

bool is_valid_date(int year, int month, int day, time_t& t) {
    if (year < 1900 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    std::tm timeinfo;
    timeinfo.tm_year  = year - 1900;
    timeinfo.tm_mon   = month - 1;
    timeinfo.tm_mday  = day;
    timeinfo.tm_hour  = 0;
    timeinfo.tm_min   = 0;
    timeinfo.tm_sec   = 0;
    timeinfo.tm_isdst = -1;

    t = std::mktime(&timeinfo);
    if (t == -1) {
        return false;
    }

    return timeinfo.tm_year == year - 1900 && timeinfo.tm_mon == month - 1 &&
           timeinfo.tm_mday == day;
}

BitcoinExchange::BitcoinExchange() {
}

BitcoinExchange::~BitcoinExchange() {
}

bool BitcoinExchange::processFile(const char* file_path) {
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) return false;

    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line;
    while (std::getline(ifs, line)) {
        std::string::size_type pipe_pos = line.find('|');
        if (pipe_pos == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date_str = ft_trim(line.substr(0, pipe_pos));
        std::string rate_str = ft_trim(line.substr(pipe_pos + 1));

        try {
            time_t date = this->parseDate(date_str);
            float  rate = this->parseRate(rate_str);

            std::map<time_t, float>::iterator it = _database.upper_bound(date);
            if (it != _database.begin()) --it;

            std::cout << date_str << " => " << rate_str << " = " << (rate * it->second)
                      << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return true;
}

bool BitcoinExchange::loadDatabase(const char* db_path /*= DATABASE_PATH*/) {
    std::ifstream ifs(db_path);
    if (!ifs.is_open()) return false;

    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line;
    while (std::getline(ifs, line)) {
        std::string::size_type semicol_pos = line.find(',');
        if (semicol_pos == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        try {
            time_t date = this->parseDate(ft_trim(line.substr(0, semicol_pos)));
            float  rate = this->parseRate(ft_trim(line.substr(semicol_pos + 1)));

            _database[date] = rate;

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    return true;
}

time_t BitcoinExchange::parseDate(const std::string& str) const {
    int    year = -1, month = -1, day = -1;
    time_t result;

    if (std::sscanf(str.c_str(), "%d-%d-%d", &year, &month, &day) != 3 ||
        !is_valid_date(year, month, day, result)) {
        throw std::runtime_error("bad input => " + str);
    }

    return result;
}

float BitcoinExchange::parseRate(const std::string& str) const {
    char*  end   = NULL;
    double value = std::strtod(str.c_str(), &end);

    if (errno == ERANGE || value > INT_MAX) {
        throw std::runtime_error("too large a number.");
    }

    if (end == str.c_str() || *end != '\0') {
        throw std::runtime_error("invalid value: " + str);
    }

    if (value < 0) {
        throw std::runtime_error("not a positive number.");
    }
    return static_cast<float>(value);
}