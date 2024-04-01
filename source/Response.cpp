/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 14:16:00 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/31 17:24:27 by aaghbal          ###   ########.fr       */
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
    std::string msg = ":IRCsERVER 404 " + this->clients[i].get_nickname() + " " + this->clients[i].split_targ[j];
    msg += " :Cannot send to channel\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
}

void Server::Err_NeedMoreParam(int i)
{
    std::string msg = ":IRCsERVER 461" + this->clients[i].get_nickname() + " " + this->clients[i].cmd[0] + " :Not enough parameters\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
}

void Server::ErrBadChannelKey(int i, int k)
{
    std::string info = this->clients[i].get_nickname() + " " + this->clients[i].split_targ[k];
    std::string msg = ":IRCsERVER 475 " + info + " :Cannot join channel, you need the correct key (+k)\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
} 

void Server::Err_AlreadRegistred(int i)
{
    this->unk_com = false;
    std::string msg = ":IRCsERVER 462 " + this->clients[i].get_nickname() + " :You may not reregister\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    
}

void Server::No_NicknameGiven(int i)
{
    this->unk_com = false;
    std::string msg = ":IRCsERVER 431 " + this->clients[i].get_nickname() + " :No nickname given\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    
}

void Server::Err_NotRegistered(int i)
{
    this->unk_com = false;
    std::string msg = ":IRCsERVER 451 " + this->clients[i].cmd[0] + " :You have not registered\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    
}

void Server::ErrOneusNickname(int i)
{
    this->unk_com = false;
    std::string msg;
    if (!this->clients[i].get_nickname().empty())
        msg = ":IRCsERVER 432 " + this->clients[i].get_nickname() + " " + this->clients[i].cmd[1] + " :Erroneous Nickname\r\n";
    else
        msg = ":IRCsERVER 432 * " + this->clients[i].cmd[1] + " :Erroneous Nickname\r\n";
    
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    
}

void Server::Command_Responses(int i)
{
    std::string msg = ":IRCsERVER 001 " + this->clients[i].get_nickname() + WELCOME_MSG + this->clients[i].get_nickname();
    msg += " [!" + this->clients[i].cmd[1] + "@" +inet_ntoa(this->client_info[this->clients[i].get_fd_client()].sin_addr) + "]\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    msg = ":IRCsERVER 002 " + this->clients[i].get_nickname() + " :Your host is IRCsERVER, running version 1.0.0\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    msg = ":IRCsERVER 003 " + this->clients[i].get_nickname() + " :This server was created Thu Feb 29, 2024 at 17:50:31\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    msg = ":IRCsERVER 004 " + this->clients[i].get_nickname() + " IRCsERVER 1.0.0 itkol kol\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
}