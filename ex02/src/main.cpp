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

#include <iostream>
#include <ostream>

#include "../../ex00/inc/BitcoinExchange.hpp"
#include "../inc/PmergeMe.hpp"

int main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cout << ERR_COLOR << "Usage: ./PmergeMe <integer sequence>" << RESET << std::endl;
		return 1;
	}
	if (ac == 2)
	{
		std::cout << "Are you really trying to sort 1 number..." << std::endl;
		return 0;
	}
	PmergeMe data;
	int i = 1;
	while (i < ac)
	{
		if (data.parseInput(av[i]) == false)
		{
			std::cout << ERR_COLOR << "Error when trying to parse : " << av[i] << RESET << std::endl;
			return 1;
		}
		i++;
	}
	data.solve();
}
