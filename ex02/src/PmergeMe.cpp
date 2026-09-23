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

#include <algorithm>
#include <sstream>
#include <climits>
#include <iostream>

PmergeMe::PmergeMe() : _standAlone(0), _odd(false) {}

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

// void PmergeMe::printPairs(std::vector<Pair> pairs)
// {
// 	for (std::vector<Pair>::iterator it = pairs.begin();
// 		 it != pairs.end(); ++it)
// 	{
// 		std::cout << "("
// 				  << YELLOW << it->small << RESET << ", "
// 				  << RED << it->large << RESET << ") ";
// 	}
// 	if (this->_odd)
// 	{
// 		std::cout << "("<< BLUE << this->_standAlone << RESET <<")"<< std::endl;
// 	}
// 	std::cout << std::endl;
// }
//
// bool comparePairs(const Pair& a, const Pair& b)
// {
// 	return a.large < b.large;
// }

// template <typename Container>
// Container buildMainChain(const std::vector<Pair>& pairs)
// {
// 	Container mainChain;
//
// 	mainChain.push_back(pairs[0].small);
// 	for (size_t i = 0; i < pairs.size(); ++i)
// 		mainChain.push_back(pairs[i].large);
// 	return mainChain;
// }

template <typename Container>
void swapGroups(Container& container,
				size_t first,
				size_t second,
				size_t groupSize)
{
	for (size_t i = 0; i < groupSize; i++)
		std::swap(container[first * groupSize + i], container[second * groupSize + i]);
}

template <typename Container>
void PmergeMe::createPairs(Container& container, int size)
{
	if (size <= 0)
		return;

	const size_t groupSize = static_cast<size_t>(size);

	// Il faut au moins deux groupes complets pour pouvoir les comparer.
	if (container.size() < groupSize * 2)
		return;

	/*
	 * On parcourt les groupes deux par deux.
	 *
	 * Exemple avec size = 2 :
	 *
	 * [4 1] [3 2] [5 6] [8 7]
	 *    ^      ^
	 *
	 * On compare les deux groupes en regardant leur dernier élément.
	 */
	for (size_t i = 0;
		 i + groupSize * 2 <= container.size();
		 i += groupSize * 2)
	{
		size_t first = i / groupSize;
		size_t second = first + 1;

		// Le plus grand élément du groupe est à sa fin,
		// puisque les niveaux précédents ont déjà ordonné les groupes.
		if (container[i + groupSize - 1]
			> container[i + groupSize * 2 - 1])
		{
			swapGroups(container, first, second, groupSize);
		}
	}

	// Niveau suivant : les groupes font maintenant 2 * size.
	createPairs(container, size * 2);
}

void PmergeMe::solve()
{
	createPairs(this->_vector, 1);
	printContainer(1);
	// swapGroups(this->_vector, 0, 1, 1);
	// printContainer(1);
}

template <typename Container>
std::vector<Pair> PmergeMe::makePairsV(const Container& container)
{
	std::vector<Pair> pairs;

	typename Container::const_iterator it = container.begin();

	while (it != container.end())
	{
		Pair p;

		int first = *it;
		++it;

		if (it == container.end())
		{
			this->_standAlone = first;
			this->_odd = true;
			break;
		}

		int second = *it;
		++it;

		if (first < second)
		{
			p.small = first;
			p.large = second;
		}
		else
		{
			p.small = second;
			p.large = first;
		}

		pairs.push_back(p);
	}

	return pairs;
}
