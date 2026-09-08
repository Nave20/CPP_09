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

#include <complex>
#include <fstream>
#include <iostream>
#include "../inc/BitcoinExchange.hpp"

std::string	trim_space(std::string str)
{
	std::string::size_type start = 0;
	std::string::size_type end = str.size();

	while (start < end && std::isspace(static_cast<unsigned char>(str[start])))
		++start;

	while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
		--end;

	return str.substr(start, end - start);
}

std::string	string_selector(std::string str, int strt, int end)
{
	return str.substr(strt, end);
}

bool valid_date(std::string date)
{
	int daysInMonth[12] =
	{
		31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31
	};
	int daysInMonthbis[12] =
	{
		31, 29, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31
	};

	int i = 0;
	if (date[4] != '-' || date[7] != '-')
		return false;
	while (date[i])
	{
		if (i == 4 || i == 7)
		{
			i++;
			continue;
		}
		if (!isdigit(date[i]))
			return false;
		i++;
	}
	if (i != 10)
		return false;
	int year = atoi(string_selector(date, 0, 4).c_str());
	int month = atoi(string_selector(date, 5, 7).c_str());
	int day = atoi(string_selector(date, 8, 10).c_str());
	if (month < 1 || month > 12 || day < 1)
		return false;
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
	{
		if (day > daysInMonthbis[month - 1])
			return false;
	}
	else
	{
		if (day > daysInMonth[month - 1])
			return false;
	}
	return true;
}

int main(int argc, char **argv)
{
	(void)argv;
	if (argc != 2)
	{
		std::cout << ERR_COLOR <<"Usage:" << std::endl
		<<"->  ./btc <filename>" << RESET << std::endl;
		return 1;
	}
	try
	{
		BitcoinExchange data;
		try
		{
			std::ifstream infile(argv[1]);
			if (!infile.is_open())
			{
				std::cout << ERR_COLOR <<"Error opening "<< argv[1] << RESET << std::endl;
				throw std::runtime_error("Error opening input file");
			}
			std::string line;
			getline(infile, line);
			if (line != "date | value")
			{
				std::cout << ERR_COLOR <<"Wrong header for input." << RESET << std::endl;
			}
			while (getline(infile, line))
			{
				std::string::size_type pos = line.find('|');
				if (pos == std::string::npos)
				{
					std::cout << ERR_COLOR <<"Error: bad input => " << line << RESET << std::endl;
					continue;
				}
				std::string date = line.substr(0, pos);
				std::string valueStr = line.substr(pos + 1);
				date = trim_space(date);
				valueStr = trim_space(valueStr);
				if (!valid_date(date))
				{
					std::cout << ERR_COLOR <<"Error: bad input => " << line << RESET << std::endl;
					continue;
				}
				double value;
				char extra;
				std::stringstream ss(valueStr);
				if (!(ss >> value) || (ss >> extra))
				{
					std::cout << ERR_COLOR <<"Error: not a valid value => " << valueStr << RESET << std::endl;
					continue;
				}
				if (value < 0.0)
				{
					std::cout << ERR_COLOR <<"Error: not a positive number => " << value << RESET << std::endl;
					continue;
				}
				if (value > 1000.0)
				{
					std::cout << ERR_COLOR <<"Error: too large of a number => " << value << RESET << std::endl;
					continue;
				}
				double amount = data.returnAmount(date, value);
				std::cout << BLUE << date << " => " << value << " = " << BOLD << YELLOW << amount << RESET << std::endl;
			}
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	catch (std::exception& e) {std::cout << e.what() << std::endl;}
}