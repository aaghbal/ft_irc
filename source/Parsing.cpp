/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 22:54:01 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/31 17:20:21 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

int count_char(std::string &port)
{
    Error e;
    int c = 0;
    for (size_t i = 0; i < port.size(); i++)
    {
        if (port[i] == '+')
          c++;  
    }
    if (c > 1)
        e.ErrorPortSyn();
    return (c);
}

void parsing(std::string password, std::string port)
{
    Error e;
    if (port.find_first_not_of("0123456789+") != std::string::npos)
        e.ErrorPort();
    if (count_char(port) == 1 && port[0] != '+')
        e.ErrorPortSyn();
    if (password.size() > 9 || password.size() < 2)
        e.ErrorSizePass();
    if (port.size() == 0)
        e.ErrorSizePort();
}

void    parsing_port(int port)
{
    Error e;
    if (port < 1 || port > 65535)
        e.ErrorRangePort();
}

std::vector<std::string> Server::split_cmd(std::string &cmd)
{
    std::vector<std::string>    MySplitData;
    std::string                 token;

    for(size_t i = 0; i < cmd.size() ; i++)
    {
        char c = cmd[i];
        if(c != ' ' && c != '\t' && c != '\n' && c != '\r')
            token += c;
        else if (!token.empty())
        {
            MySplitData.push_back(token);
            token.clear();
        }
    }
    if (!token.empty())
        MySplitData.push_back(token);
    cmd.clear();
    return (MySplitData);
}

void Server::split_target(std::string &cmd, int fd, int flag)
{
    if (this->clients[fd].split_pass.size() > 0)
        this->clients[fd].split_pass.clear();
    std::string token = "";
    for(size_t i = 0; i < cmd.size(); i++)
    {
        char c = cmd[i];
        if(c != ',')
            token += c;
        else if (!token.empty())
        {
            if (flag)
                this->clients[fd].split_targ.push_back(token);
            else
                this->clients[fd].split_pass.push_back(token);
                
            token.clear();
        }
    }
    if (!token.empty() && flag)
        this->clients[fd].split_targ.push_back(token);
    else if (!token.empty())
        this->clients[fd].split_pass.push_back(token);
        
}

void    Server::to_upercase(std::string &cmd)
{
    for (size_t i = 0; i < cmd.size(); i++)
    {
        cmd[i] = toupper(cmd[i]);
    }
}

void Server::password_channel(std::string &pass)
{
    if (pass.size() > 24)
        for (size_t i = 24; i < pass.size(); i++)
        {
                pass.erase(i);
        }
}

bool Server::parsing_nickname(int i)
{
    if (this->clients[i].cmd[1][0] == '#')
    {
        ErrOneusNickname(i);
        return  true;
    }
    else if (isdigit(this->clients[i].cmd[1][0]))
    {
        ErrOneusNickname(i);
        return  true;
    }
    return  false;
}