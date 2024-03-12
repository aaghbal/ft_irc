/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:25:12 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/12 13:16:34 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void Client::set_nickname(const std::string &niname)
{
    this->nickname = niname;
}

void Client::set_username(const std::string &usname)
{
    this->username = usname;
}

void Client::set_fd_client(int fd)
{
    this->fd_client = fd;       
}

int Client::get_fd_client(void)
{
    return(this->fd_client);
}

void Client::set_password(const std::string & pass)
{
    this->password = pass;
}

std::string & Client::get_password(void)
{
    return (this->password);
}

// void Client::init_client(void)
// {
//     send
    
// }
