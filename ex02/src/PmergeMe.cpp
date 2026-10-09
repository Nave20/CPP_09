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
		std::cout << RED;
		std::vector<int>::iterator it = this->_vector.begin();
		for (; it != this->_vector.end(); ++it)
			std::cout << *it << " ";
		std::cout << RESET <<std::endl;
	}
	else
	{
		std::cout << BLUE;
		std::deque<int>::iterator it = this->_deque.begin();
		for (; it != this->_deque.end(); ++it)
			std::cout << *it << " ";
		std::cout << RESET <<std::endl;
	}
	// std::cout << CYAN << "reclvl :" << _recLvl << RESET << std::endl;
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
		size_t i = container.size() - 1;

		this->_standAlone = container[i];
		this->_odd = true;

		container.erase(container.begin() + i);
	}
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

	// std::cout << "insertGroup:"
	// 		  << " fromGroup=" << GroupNBRfrom
	// 		  << " toGroup=" << GroupNBRto
	// 		  << " fromIndex=" << fromIndex
	// 		  << " toIndex=" << toIndex
	// 		  << " groupSize=" << groupSize
	// 		  << " from.size=" << from.size()
	// 		  << " to.size=" << to.size()
	// 		  << std::endl;

	if (fromIndex + groupSize > from.size())
	{
		// std::cout << "ERROR FROM" << std::endl;
		return;
	}

	if (toIndex > to.size())
	{
		// std::cout << "ERROR TO" << std::endl;
		return;
	}

	to.insert(
		to.begin() + toIndex,
		from.begin() + fromIndex,
		from.begin() + fromIndex + groupSize
	);
}

template<typename Container>
Container extractBigNumbers(Container &container, size_t box_number, size_t box_size)
{
	Container bigNbr;

	for (size_t box = 0; box < box_number; ++box)
	{
		size_t index = box * box_size + (box_size - 1);

		if (index >= container.size())
			break;

		bigNbr.push_back(container[index]);
	}

	return bigNbr;
}

template <typename Container>
size_t binarySearch(const Container& container, int number, size_t maxIndex)
{
	size_t left = 0;
	size_t right = maxIndex;

	while (left < right)
	{
		size_t middle = left + (right - left) / 2;

		if (container[middle] < number)
			left = middle + 1;
		else
			right = middle;
	}

	return left;
}

template<typename Container>
void PmergeMe::reversePairingVector(Container &container, int recursionDepth)
{
	if (recursionDepth < 0)
		return;
	size_t box_size = 1 << recursionDepth;
	size_t box_number = container.size() / box_size;
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
	Container bigNbr = extractBigNumbers(container, box_number, box_size);
	size_t maxRank = (box_number + 1) / 2;
	size_t previous = 1;
	size_t previousJacob = 1;
	size_t jacob = 3;
	while (previous < maxRank)
	{
	    size_t high = jacob;
	    if (high > maxRank)
	        high = maxRank;
	    size_t low = previous + 1;
	    size_t rank = high;

	    while (rank >= low)
	    {
	        size_t index = 2 * (rank - 1);
	        Container bigInserted =
	            extractBigNumbers(MainChain, inserted, box_size);
	        if (index >= bigNbr.size())
	            return;

	        size_t numberChecked = bigNbr[index];
	        size_t upperBound = 0;
	        bool hasPartner = (index + 1 < box_number);
	        if (hasPartner)
	            upperBound = bigNbr[index + 1];
	        size_t maxIndex = bigInserted.size();
	        if (hasPartner)
	        {
	            bool found = false;

	            for (size_t i = 0; i < bigInserted.size(); ++i)
	            {
	                if (bigInserted[i] == static_cast<int>(upperBound))
	                {
	                    maxIndex = i;
	                    found = true;
	                    break;
	                }
	            }
	            if (!found)
	                return;
	        }
	        size_t insertion = binarySearch(bigInserted, numberChecked, maxIndex);
	        insertGroup(container, MainChain, index, insertion, box_size);
	        ++inserted;
	        if (rank == low)
	            break;
	        --rank;
	    }
	    previous = high;
	    size_t nextJacob = jacob + 2 * previousJacob;
	    previousJacob = jacob;
	    jacob = nextJacob;
	}
	for (size_t i = box_number * box_size; i < container.size(); i++)
		MainChain.push_back(container[i]);
	this->_vector = MainChain;
	reversePairingVector(this->_vector, recursionDepth - 1);
}

template<typename Container>
void PmergeMe::reversePairingDeque(Container &container, int recursionDepth)
{
	if (recursionDepth < 0)
		return;
	size_t box_size = 1 << recursionDepth;
	size_t box_number = container.size() / box_size;
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
	Container bigNbr = extractBigNumbers(container, box_number, box_size);
	size_t maxRank = (box_number + 1) / 2;
	size_t previous = 1;
	size_t previousJacob = 1;
	size_t jacob = 3;
	while (previous < maxRank)
	{
	    size_t high = jacob;
	    if (high > maxRank)
	        high = maxRank;
	    size_t low = previous + 1;
	    size_t rank = high;

	    while (rank >= low)
	    {
	        size_t index = 2 * (rank - 1);
	        Container bigInserted =
	            extractBigNumbers(MainChain, inserted, box_size);
	        if (index >= bigNbr.size())
	            return;

	        size_t numberChecked = bigNbr[index];
	        size_t upperBound = 0;
	        bool hasPartner = (index + 1 < box_number);
	        if (hasPartner)
	            upperBound = bigNbr[index + 1];
	        size_t maxIndex = bigInserted.size();
	        if (hasPartner)
	        {
	            bool found = false;

	            for (size_t i = 0; i < bigInserted.size(); ++i)
	            {
	                if (bigInserted[i] == static_cast<int>(upperBound))
	                {
	                    maxIndex = i;
	                    found = true;
	                    break;
	                }
	            }
	            if (!found)
	                return;
	        }
	        size_t insertion = binarySearch(bigInserted, numberChecked, maxIndex);
	        insertGroup(container, MainChain, index, insertion, box_size);
	        ++inserted;
	        if (rank == low)
	            break;
	        --rank;
	    }
	    previous = high;
	    size_t nextJacob = jacob + 2 * previousJacob;
	    previousJacob = jacob;
	    jacob = nextJacob;
	}
	for (size_t i = box_number * box_size; i < container.size(); i++)
		MainChain.push_back(container[i]);
	this->_deque = MainChain;
	reversePairingDeque(this->_deque, recursionDepth - 1);
}

template <typename Container>
void insertStraggler(Container& sorted, int straggler)
{
	typename Container::iterator pos = sorted.begin();
	typename Container::iterator end = sorted.end();

	while (pos < end)
	{
		typename Container::iterator middle = pos + (end - pos) / 2;

		if (*middle < straggler)
			pos = middle + 1;
		else
			end = middle;
	}

	sorted.insert(pos, straggler);
}

template <typename Container>
void validate(Container input, Container result)
{
	bool sorted = true;
	for (size_t i = 1; i < result.size(); ++i)
	{
		if (result[i - 1] > result[i])
		{
			sorted = false;
			break;
		}
	}
	bool sameSize = (result.size() == input.size());

	Container expected = input;
	Container actual = result;

	std::sort(expected.begin(), expected.end());
	std::sort(actual.begin(), actual.end());

	bool sameValues = (expected == actual);

	std::cout << GREEN << "--------------" << std::endl;
	std::cout << "Sorted : " << (sorted ? "OK" : "Failure") << std::endl;
	std::cout << "Size : " << (sameSize ? "OK" : "Failure") << std::endl;
	std::cout << "Value validation : "
			  << (sameValues ? "OK" : "Failure") << std::endl;
	std::cout << "--------------" << RESET << std::endl;
}

void PmergeMe::solve()
{
	std::vector<int> container = this->_vector;
	std::deque<int> container2 = this->_deque;

	stragglerHandling(this->_vector);
	createPairs(this->_vector, 1);
	reversePairingVector(this->_vector, this->_recLvl - 1);
	if (this->_odd == true)
		insertStraggler(this->_vector, this->_standAlone);

	this->_recLvl = 0;
	createPairs(this->_deque, 1);
	reversePairingDeque(this->_deque, this->_recLvl - 1);


	printContainer(1);
	printContainer(0);

	validate(container, this->_vector);
	validate(container2, this->_deque);
}
