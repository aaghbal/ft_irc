/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:26:23 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/27 22:56:48 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Header.hpp"

bool Channel::is_invited(int fd)
{
    for (size_t i = 0; i < this->invited.size(); i++)
    {
        if (this->invited[i] == fd)
            return true;
    }
    return false;
}
void Channel::erase_operat(int fd)
{
    for (size_t i = 0; i < this->operat.size(); i++)
    {
        if (this->operat[i] == fd)
        {
            this->operat.erase(this->operat.begin() + i);
            return ;
        }
    }
}

void Channel::set_name(std::string const &name)
{
    this->_Name = name;
}

void Channel::Add_Client(Client &cli)
{
    this->_Client.push_back(cli);
}

std::string &Channel::get_name_channel(void)
{
    return (this->_Name);
}

bool    Channel::is_operator(int fd)
{
    for (size_t i = 0; i < this->operat.size(); i++)
    {
        if(this->operat[i] == fd)
            return true;
    }
    return false;
}

std::string &Channel::get_topic(void)
{
    return this->topic;
}

bool Channel::joined_in_channel(int fd)
{
    for (size_t i = 0; i < this->_Client.size() ; i++)
    {
        if (this->_Client[i].get_fd_client() == fd)
            return true;
    }
    return false;
}
