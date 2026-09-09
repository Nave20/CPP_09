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

#include "../inc/PmergeMe.hpp"

#include <sstream>
#include <climits>
#include <iostream>

PmergeMe::PmergeMe() {}

PmergeMe::~PmergeMe() {}

bool PmergeMe::parseInput(std::string input)
{
	std::stringstream ss(input);
	long temp;
	char err;

	if (!(ss >> temp) || (ss >> err))
		return false;
	if (temp < 0 || temp > INT_MAX)
		return false;

	this->_deque.push_back(static_cast<int>(temp));
	this->_vector.push_back(static_cast<int>(temp));
	return true;
}

void PmergeMe::printContainer(bool b)
{
	if (b == true)
	{
		std::vector<int>::iterator it = this->_vector.begin();
		for (; it != this->_vector.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
	else
	{
		std::deque<int>::iterator it = this->_deque.begin();
		for (; it != this->_deque.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
}