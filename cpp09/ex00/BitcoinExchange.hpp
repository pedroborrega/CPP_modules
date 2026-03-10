/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 18:56:26 by psantos-          #+#    #+#             */
/*   Updated: 2025/12/12 00:05:49 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>

class BitcoinExchange
{
	private:
		std::map<std::string,float> _data;

		bool isValidDate(const std::string& date) const;
	public:	
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void loadDatabase(const std::string& filename);
    	float getRate(const std::string& date) const;
		std::string getClosestDate(const std::string& date) const;
};

void parseInput(const std::string& line, std::string& dateStr, float& value);
