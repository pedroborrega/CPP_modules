/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:30:09 by psantos-          #+#    #+#             */
/*   Updated: 2025/12/12 11:29:23 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>


std::string trim(const std::string& s)
{
	size_t start = s.find_first_not_of(" \t");
	if (start == std::string::npos)
		return "";
	size_t end = s.find_last_not_of(" \t");
	return s.substr(start, end - start + 1);
}

bool parseIntStrict(const std::string& s, int& out)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] < '0' || s[i] > '9')
			return false;
	}
	std::stringstream ss(s);
	ss >> out;
	return !ss.fail() && ss.eof();
}

bool parseFloatStrict(const std::string& s, float& out)
{
	std::stringstream ss(s);
	ss >> out;
	if (ss.fail())
		return false;
	char extra;
	if (ss >> extra)
		return false;
	return true;
}

int daysInMonth(int year, int month)
{
	static const int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if (month == 2)
	{
		bool leap = (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
		return leap ? 29 : 28;
	}
	return days[month - 1];
}

bool isValidDateImpl(const std::string& date)
{
	if (date.size() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;
	int year, month, day;
	if (!parseIntStrict(date.substr(0, 4), year))
		return false;
	if (!parseIntStrict(date.substr(5, 2), month))
		return false;
	if (!parseIntStrict(date.substr(8, 2), day))
		return false;
	if (month < 1 || month > 12)
		return false;
	if (day < 1 || day > daysInMonth(year, month))
		return false;
	return true;
}

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _data(other._data) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
        _data = other._data;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
	return isValidDateImpl(date);
}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Error: could not open database file");
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string date, valueStr;
        if (!std::getline(ss, date, ',') || !std::getline(ss, valueStr))
            continue;
        if (!isValidDate(date))
            continue;
		float rate;
		if (!parseFloatStrict(valueStr, rate))
			continue;
		_data[date] = rate;
    }
}

float BitcoinExchange::getRate(const std::string& date) const {
    if (_data.empty())
        throw std::runtime_error("Database is empty");

	std::map<std::string,float>::const_iterator it = _data.lower_bound(date);

    if (it == _data.end())
	{
		std::map<std::string,float>::const_iterator last = _data.end();
		--last;
        return last->second;
	}

    if (it->first == date)
        return it->second;

    if (it == _data.begin())
        throw std::runtime_error("Error: bad input");

    std::map<std::string,float>::const_iterator prev = it;
	--prev;
	return prev->second;
}

std::string BitcoinExchange::getClosestDate(const std::string& date) const
{
	if (_data.empty())
		throw std::runtime_error("Database is empty");

	std::map<std::string,float>::const_iterator it = _data.lower_bound(date);

	if (it == _data.end())
	{
		std::map<std::string,float>::const_iterator last = _data.end();
		--last;
		return last->first;
	}
	if (it->first == date)
		return it->first;
	if (it == _data.begin())
		throw std::runtime_error("Error: bad input");
	std::map<std::string,float>::const_iterator prev = it;
	--prev;
	return prev->first;
}

void parseInput(const std::string& line, std::string& dateStr, float& value)
{
    std::stringstream ss(line);
    std::string valueStr;

    if (!std::getline(ss, dateStr, '|') || !std::getline(ss, valueStr))
        throw std::runtime_error("Error: bad input");

	dateStr = trim(dateStr);
	valueStr = trim(valueStr);
	if (dateStr.empty() || valueStr.empty())
		throw std::runtime_error("Error: bad input");

	if (!isValidDateImpl(dateStr))
		throw std::runtime_error("Error: bad input");

	if (!parseFloatStrict(valueStr, value))
		throw std::runtime_error("Error: bad input");
    if (value < 0)
        throw std::runtime_error("Error: not a positive number");
    if (value > 1000)
        throw std::runtime_error("Error: too large a number");
}
