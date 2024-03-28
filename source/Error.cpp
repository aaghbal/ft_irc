/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:32:56 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/28 12:50:09 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Error.hpp"

void Error::ErrorArgument(void) const
{
    std::cerr << "Error Argument! : Correct syntax ./ircserv <port> <password>" << std::endl;
    exit(1);
}

void Error::ErrorGetAddrInof(void) const
{
    std::cerr << "Error! : there is problem with getaddrinfo fucntion" << std::endl;
    exit(1);
}

void Error::ErrorSocket(void) const
{
    std::cerr << "Error! : there is problem with socket fucntion" << std::endl;
    exit(1);
}

void Error::ErrorSetsockopt(void) const
{
    std::cerr << "Error! : there is problem with setsockopt fucntion" << std::endl;
    exit(1);
}
void Error::ErrorBind(void) const
{
    std::cerr << "Error! : there is problem with bind fucntion" << std::endl;
    exit(1);
}

void Error::ErrorAccept(void) const
{
    std::cerr << "Error! : there is problem with accept fucntion" << std::endl;
    exit(1);
}

void Error::ErrorSizePass(void) const
{
    std::cerr << "Error ! the password must contain at least 2 characters and maximum of 9 characters\n";
    exit(1);
}
void Error::ErrorPort(void) const
{
    std::cerr << "Error ! port must contain number only in this range [0,65535]" << std::endl;
    exit(1);
}
void Error::ErrorSizePort(void) const
{
    std::cerr << "Error ! port is empty, enter a number in this number [0,65535]" << std::endl;
    exit(1);  
}
void Error::ErrorRangePort(void) const
{
    std::cerr << "Error ! out of range port [0,65535]." << std::endl;
    exit(1);
}
void Error::ErrorPortSyn(void) const
{
    std::cerr << "Error ! port syntax incorect" << std::endl;
    exit(1);
}

void Error::ErrorListen(void) const
{
    std::cerr << "Error! : there is problem with listen fucntion" << std::endl;
    exit(1);
}
