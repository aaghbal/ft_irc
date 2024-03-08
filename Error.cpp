/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:32:56 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/08 11:10:08 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"

void Error::ErrorArgument(void) const
{
    std::cerr << "Error Argument! : Correct syntax ./ircserv <port> <password>" << std::endl;
}

void Error::ErrorGetAddrInof(void) const
{
    std::cerr << "Error! : there is problem with getaddrinfo fucntion" << std::endl;
}

void Error::ErrorSocket(void) const
{
    std::cerr << "Error! : there is problem with socket fucntion" << std::endl;
}

void Error::ErrorBind(void) const
{
    std::cerr << "Error! : there is problem with bind fucntion" << std::endl;
}

void Error::ErrorAccept(void) const
{
    std::cerr << "Error! : there is problem with accept fucntion" << std::endl;
}

void Error::ErrorListen(void) const
{
    std::cerr << "Error! : there is problem with listen fucntion" << std::endl;
}
