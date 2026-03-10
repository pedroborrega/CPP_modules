/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psantos- <psantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 12:35:47 by psantos-          #+#    #+#             */
/*   Updated: 2025/12/13 15:08:06 by psantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <deque>
#include <string>

class PmergeMe
{
	private:
		std::vector<int> _vec;
		std::deque<int>  _deq;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe& operator=(const PmergeMe &other);
		~PmergeMe();

		void setData(const std::vector<int>& vec, const std::deque<int>& deq);
		void sortAndDisplay();
};

bool isPositiveInteger(const std::string& s);
int  toInt(const std::string& s);
void parseInput(int argc, char** argv, std::vector<int>& vec, std::deque<int>& deq);
