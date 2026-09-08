/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*                                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpirotti <vpirotti@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   GitHub : @Nave20                                  #+#    #+#             */
/*   28 is the new 42                                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/BitcoinExchange.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>

BitcoinExchange::BitcoinExchange()
{
	std::ifstream infile("data.csv");
	if (!infile.is_open())
	{
		std::cout << ERR_COLOR << "Error opening data sheet:" << std::endl
		<< "->  Please make sure data.csv is at the root of the project." << RESET << std::endl;
		throw std::runtime_error("Error opening data.csv");
	}
	std::string line;
	std::getline(infile, line);
	while (std::getline(infile, line))
	{
		std::string key;
		std::string stringValue;
		double		value = 0;

		key = line.substr(0, line.find_first_of(','));
		stringValue = line.substr(line.find_first_of(',') + 1).c_str();
		value = atof(stringValue.c_str());
		this->_map[key] = value;
	}
	infile.close();
}

BitcoinExchange::~BitcoinExchange() {}

double BitcoinExchange::returnAmount(std::string str, double nbr)
{
	std::map<std::string, double>::iterator it = this->_map.lower_bound(str);
	if (it != _map.end() && it->first == str)
	{
		return it->second * nbr;
	}
	else
	{
		if (it == _map.begin())
		{
			throw NoPrevDate();
		}
		else
		{
			--it;
			return it->second * nbr;
		}
	}
}
