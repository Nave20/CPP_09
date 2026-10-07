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

PmergeMe::PmergeMe() : _standAlone(0), _odd(false), _recLvl(0) {}

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
	std::cout << "reclvl :" << _recLvl << std::endl;
}

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
void insertGroup(Container& from,
				 Container& to,
				 size_t GroupNBRfrom,
				 size_t GroupNBRto,
				 size_t groupSize)
{
	size_t fromIndex = GroupNBRfrom * groupSize;
	size_t toIndex   = GroupNBRto * groupSize;

	to.insert(
		to.begin() + toIndex,
		from.begin() + fromIndex,
		from.begin() + fromIndex + groupSize
	);
}

template <typename Container>
void PmergeMe::createPairs(Container& container, int size)
{
	if (size <= 0)
		return;

	const size_t groupSize = static_cast<size_t>(size);
	if (container.size() < groupSize * 2)
		return;

	this->_recLvl += 1;

	for (size_t i = 0;
		 i + groupSize * 2 <= container.size();
		 i += groupSize * 2)
	{
		size_t first = i / groupSize;
		size_t second = first + 1;
		if (container[i + groupSize - 1]
			> container[i + groupSize * 2 - 1])
		{
			swapGroups(container, first, second, groupSize);
		}
	}
	createPairs(container, size * 2);
}

template<typename Container>
void PmergeMe::stragglerHandling(Container &container)
{
	if (container.size() % 2 != 0)
	{
		size_t i = 0;
		for (; i < container.size(); i++);
		this->_standAlone = container[i];
		this->_odd = true;
		container.erase(container.begin() + i - 1, container.end());
	}
}

template<typename Container>
void PmergeMe::reversePairing(Container &container, int recursionDepth)
{
	if (recursionDepth == 0)
		return;
	size_t box_size = 1 << recursionDepth;
	size_t box_number = container.size() / box_size;
							std::cout
							<< BLUE << "box size :" << box_size << std::endl
							<< YELLOW << "box number : " << box_number << RESET << std::endl;
	Container MainChain;
	size_t inserted = 0;
	for (size_t i = 0; i < box_number; i++)
	{
		if (i == 0)
		{
			insertGroup(container, MainChain, i, inserted, box_size);
			inserted++;
		}
		else if (i % 2 == 1)
		{
			insertGroup(container, MainChain, i, inserted, box_size);
			inserted++;
		}
	}
	int JacobSthal = 3;
	int PrevRank = 1;
	int Index = 3;
	while (inserted < box_number)
	{
		if (Index == PrevRank)
		{
			int a = JacobSthal;
			JacobSthal = JacobSthal + PrevRank * 2;
			PrevRank = a;
			Index = JacobSthal;
		} //PAS BON car l'indice decremente de 2, faire un tableau des grands uniquement ?
		size_t numberChecked = box_size * (Index + 2) - 1;
		numberChecked = container[numberChecked];
		std::cout << MAGENTA << "Number checked : " << numberChecked << RESET << std::endl;



		// numberChecked = container[numberChecked];
		// size_t UpperBound = box_size * (Index + 3);
		// if (UpperBound > container.size())
		// 	UpperBound = -1;
		// else
		// 	UpperBound = container[UpperBound];


		// std::cout << GREEN << "UpperBound : " << UpperBound << RESET << std::endl;
		// std::cout << CYAN << "Inserted : " << inserted << RESET << std::endl;
		Index -= 2;
		inserted++;
	}

	this->_vector = MainChain;
}

void PmergeMe::solve()
{
	stragglerHandling(this->_vector);
	// printContainer(1);
	createPairs(this->_vector, 1);
	printContainer(1);
	reversePairing(this->_vector, this->_recLvl - 2);
	printContainer(1);
	// printContainer(1);
}
