/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:25:12 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/15 15:35:21 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
{
    this->authenticate = false;
    this->nickname = "";
    this->username = "";
    this->num_pass = 0;
    this->info_client_fin = false;
}
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

void Client::set_password(const std::string & pass)
{
    this->password = pass;
}

int Client::get_fd_client(void)
{
    return(this->fd_client);
}


std::string &Client::get_nickname(void)
{
    return (this->nickname);
}

std::string &Client::get_username(void)
{
    return (this->username);
}

std::string & Client::get_password(void)
{
    return (this->password);
}
