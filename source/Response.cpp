/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 14:16:00 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/30 23:42:45 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server::get_response_privmsg(std::string &msg, std::string &cmd, int i, int fd)
{
    msg += ":" + this->clients[i].get_nickname();
    msg += "!~";
    msg += this->clients[i].get_username();
    msg += '@';
    struct sockaddr_in t = this->client_info[fd];
    msg += inet_ntoa(t.sin_addr);
    msg += " PRIVMSG ";
    msg += cmd + " ";
}

void Server::get_response_join(std::string &cmd, int i, int fd)
{
    std::string msg;
    msg += ':' + this->clients[i].get_nickname();
    msg += "!~";
    msg += this->clients[i].get_username();
    msg += '@';
    struct sockaddr_in t = this->client_info[fd];
    msg += inet_ntoa(t.sin_addr);
    msg += " JOIN ";
    msg += cmd + "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void Server::get_response_nick(std::string &cmd, int i, int fd)
{
    std::string msg;
    msg += ":" + this->clients[i].get_nickname();
    msg += "!~";
    msg += this->clients[i].get_username();
    msg += '@';
    struct sockaddr_in t = this->client_info[fd];
    msg += inet_ntoa(t.sin_addr);
    msg += " NICK :";
    msg += cmd + "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void Server::GetUserChannel(std::string &msg, int ch)
{
    size_t count = this->channels[ch]._Client.size() - 1;
    for (size_t i = 0; i < this->channels[ch]._Client.size(); i++)
    {
        if (channels[ch].is_operator(channels[ch]._Client[count].get_fd_client()))
            msg += "@" + channels[ch]._Client[count].get_nickname();
        else
            msg += channels[ch]._Client[count].get_nickname();
        if (i + 1 != this->channels[ch]._Client.size())
            msg += " ";
        count--;   
    }
    msg += "\r\n";   
}

void Server::CannotendToChan(int i, int j)
{
    std::string msg = ":ircserver 404 " + this->clients[i].get_nickname() + " " + this->clients[i].split_targ[j];
    msg += " :Cannot send to channel\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
}

void Server::Err_NeedMoreParam(int i)
{
    std::string msg = ":ircserver 461 " + this->clients[i].get_nickname() + " " + this->clients[i].cmd[0] + " :Not enough parameters\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
}

void Server::ErrBadChannelKey(int i, int k)
{
    std::string info = this->clients[i].get_nickname() + " " + this->clients[i].split_targ[k];
    std::string msg = ":ircserver 475 " + info + " :Cannot join channel, you need the correct key (+k)\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
} 