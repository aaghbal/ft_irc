/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:32:56 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/01 15:33:13 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"

void Error::ErrorArgument(void) const
{
    std::cout << "Error Argument! : Correct syntax ./ircserv <port> <password>" << std::endl;
}