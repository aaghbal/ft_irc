/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:25:12 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/11 16:40:50 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void Client::set_nickname(std::string &niname)
{
    this->nickname = niname;
}

void Client::set_username(std::string &usname)
{
    this->username = usname;
}

void Client::set_fd_client(int fd)
{
    this->fd_client = fd;       
}
