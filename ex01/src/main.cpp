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
#include <sstream>
#include <string>
#include <cstdlib>

#include "../inc/RPN.hpp"

int main(int ac, char** av)
{
	if (ac != 2)
		return 1;
	if (av[1][0] == 0)
		return 1;
	std::string token;
	std::stringstream ss(av[1]);
	RPN stack;

	while (ss >> token)
	{
		if (token.length() == 2)
		{
			if (token[0] == '-' && isdigit(token[1]))
			{
				stack.push(atoi(token.c_str()));
				continue;
			}
			return 1;
		}
		if (token.length() != 1)
			return 1;
		if (isdigit(token[0]))
			stack.push(atoi(token.c_str()));
		else if (token == "*" || token == "-" || token == "+" || token == "/")
		{
			if (stack.switchOpp(token) == false)
				return 1;
		}
		else
			return 1;
	}
	if (stack.size() > 1)
		return 1;
	std::cout << stack.top() << std::endl;
	return 0;
}
