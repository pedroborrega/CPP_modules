/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:46:22 by psantos-          #+#    #+#             */
/*   Updated: 2025/12/12 00:23:49 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./btc <input_file>\n";
        return 1;
	}

    BitcoinExchange btc;
    try
    {
        btc.loadDatabase("data.csv");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile.is_open())
    {
        std::cerr << "Error: could not open file.\n";
        return 1;
    }

    std::string line;
    std::getline(infile, line);

    while (std::getline(infile, line))
    {
        std::string date;
        float value;

        try
        {
            parseInput(line, date, value);
			std::string usedDate = btc.getClosestDate(date);
            float rate = btc.getRate(usedDate);
            std::cout << usedDate << " => " << value << " = " << value * rate << std::endl;
        }
        catch (const std::exception &e)
        {
			std::string msg = e.what();
			if (msg == "Error: not a positive number." || msg == "Error: too large a number.")
				std::cerr << msg << std::endl;
			else if (msg == "Error: bad input")
				std::cerr << "Error: bad input => " << line << std::endl;
			else
				std::cerr << msg << std::endl;
        }
    }

    return 0;
}
