/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 14:50:54 by psantos-          #+#    #+#             */
/*   Updated: 2025/12/13 15:08:25 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <algorithm>
#include <sys/time.h>


// generate Jacobsthal numbers J0=0, J1=1, Jn=Jn-1+2*Jn-2
std::vector<size_t> buildJacobsthalOrder(size_t n)
{
	std::vector<size_t> order;
	if (n == 0)
		return order;

	// Track which indices are already scheduled.
	std::vector<bool> used(n, false);

	// First element is always inserted first.
	order.push_back(0);
	used[0] = true;

	// Generate Jacobsthal numbers up to n.
	std::vector<size_t> jacob;
	jacob.push_back(0);
	jacob.push_back(1);
	while (jacob.back() < n)
		jacob.push_back(jacob[jacob.size() - 1] + 2 * jacob[jacob.size() - 2]);

	for (size_t k = 2; k < jacob.size(); ++k)
	{
		size_t high = jacob[k];
		size_t low = jacob[k - 1] + 1;
		for (size_t i = high; i >= low; --i)
		{
			size_t idx = i - 1; // 1-based to 0-based
			if (idx < n && !used[idx])
			{
				order.push_back(idx);
				used[idx] = true;
			}
			if (i == low)
				break;
		}
	}

	// Append any remaining indices in increasing order.
	for (size_t i = 0; i < n; ++i)
	{
		if (!used[i])
			order.push_back(i);
	}

	return order;
}


template <typename Container>
void fordJohnson(Container& c)
{
	if (c.size() <= 1)
		return;

	Container mainChain;
	Container pend;

	for (size_t i = 0; i + 1 < c.size(); i += 2)
	{
		if (c[i] < c[i + 1])
		{
			mainChain.push_back(c[i + 1]);
			pend.push_back(c[i]);
		}
		else
		{
			mainChain.push_back(c[i]);
			pend.push_back(c[i + 1]);
		}
	}

	if (c.size() % 2 != 0)
		pend.push_back(c.back());

	std::sort(mainChain.begin(), mainChain.end());

	std::vector<size_t> order = buildJacobsthalOrder(pend.size());
	for (size_t i = 0; i < order.size(); ++i)
	{
		size_t idx = order[i];
		typename Container::iterator pos =
			std::lower_bound(mainChain.begin(), mainChain.end(), pend[idx]);
		mainChain.insert(pos, pend[idx]);
	}

	c = mainChain;
}


PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) : _vec(other._vec), _deq(other._deq) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		_vec = other._vec;
		_deq = other._deq;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::setData(const std::vector<int>& vec, const std::deque<int>& deq)
{
	_vec = vec;
	_deq = deq;
}

void PmergeMe::sortAndDisplay()
{
	std::cout << "Before: ";
	for (size_t i = 0; i < _vec.size(); ++i)
		std::cout << _vec[i] << " ";
	std::cout << std::endl;

	struct timeval start, end;
	gettimeofday(&start, NULL);
	fordJohnson(_vec);
	gettimeofday(&end, NULL);

	double vecTime =
		(end.tv_sec - start.tv_sec) * 1000000.0 +
		(end.tv_usec - start.tv_usec);

	gettimeofday(&start, NULL);
	fordJohnson(_deq);
	gettimeofday(&end, NULL);

	double deqTime =
		(end.tv_sec - start.tv_sec) * 1000000.0 +
		(end.tv_usec - start.tv_usec);

	std::cout << "After:  ";
	for (size_t i = 0; i < _vec.size(); ++i)
		std::cout << _vec[i] << " ";
	std::cout << std::endl;

	std::cout << "Time to process a range of "
			  << _vec.size()
			  << " elements with std::vector : "
			  << vecTime << " us" << std::endl;

	std::cout << "Time to process a range of "
			  << _deq.size()
			  << " elements with std::deque  : "
			  << deqTime << " us" << std::endl;
}

bool isPositiveInteger(const std::string& s)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (!std::isdigit(s[i]))
			return false;
	}
	return true;
}

int toInt(const std::string& s)
{
	std::istringstream iss(s);
	long value;
	iss >> value;

	if (iss.fail() || value <= 0 || value > INT_MAX)
		throw std::runtime_error("Error");

	return static_cast<int>(value);
}

void parseInput(int argc, char** argv, std::vector<int>& vec, std::deque<int>& deq)
{
	if (argc < 2)
		throw std::runtime_error("Error");

	for (int i = 1; i < argc; ++i)
	{
		std::string arg(argv[i]);

		if (!isPositiveInteger(arg))
			throw std::runtime_error("Error");

		int value = toInt(arg);
		vec.push_back(value);
		deq.push_back(value);
	}
}
