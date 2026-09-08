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

#include "../inc/RPN.hpp"

bool RPN::switchOpp(std::string token)
{
	if (this->size() < 2)
		return false;
	int a;
	int b;
	switch (token[0])
	{
		case '+':
			a = this->top();
			this->pop();
			b = this->top();
			this->pop();
			a += b;
			this->push(a);
			break;
		case '-':
			a = this->top();
			this->pop();
			b = this->top();
			this->pop();
			b -= a;
			this->push(b);
			break;
		case '*':
			a = this->top();
			this->pop();
			b = this->top();
			this->pop();
			a *= b;
			this->push(a);
			break;
		case '/':
			a = this->top();
			this->pop();
			b = this->top();
			this->pop();
			if (a == 0)
				return false;
			b /= a;
			this->push(b);
			break;
	}
	return true;
}

