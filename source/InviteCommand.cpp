/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:41:58 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/26 14:42:14 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server::InviteCommand(int i)
{
    std::string msg;
    if(this->clients[i].cmd.size() != 3)
    {
        msg = ":ircserver 461 INVITE :Not enough parameters\r\n";
        send(this->clients[i].get_fd_client(), "Invalid number of arguments\r\n", 29, 0);
        return;
    }
    int fd = invite_check(this->clients[i].cmd[2], this->clients[i].cmd[1], this->clients[i].get_fd_client(),i);
    if (fd == -1)
        return;

    msg = ":" + this->clients[i].get_nickname() + "!" + this->clients[i].get_username() + "@" + "localhost" + " INVITE " + this->clients[i].cmd[1] + " :" + this->clients[i].cmd[2] + "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    //invitation sent
    msg = ":ircserver 341 INVITE :Invite to channel sent\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
}

int Server::invite_check(std::string &nick, std::string &chan, int fd,int i)
{
    std::string msg;
    if (nick.empty() || chan.empty())
        return (-1);
    int fd_rec = this->client_name[nick];
    int ch = this->found_channel(chan);
    if(fd_rec == 0 || ch == -1)
    {
        msg = ":ircserver 401 INVITE :No such nick/channel\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
        return (-1);
    }
    if (check_client_channel(nick, ch, 0,0) != -1)
    {
        msg = ":ircserver 443 INVITE :is already on channel\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return (-1);
    }
    if(check_client_channel(this->clients[i].get_nickname(), ch, 0,0) == -1)
    {
        msg = ":ircserver 442 INVITE INVITE :You're not on that channel\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return (-1);
    }
    this->channels[ch].invited.push_back(fd_rec);
    return (fd_rec);
}

void Server::InviteCommand(int i)
{
    if (this->clients[i].cmd.size() != 3)
    {
        std::string msg = ":ircserver 461 INVITE :Not enough parameters\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    std::string msg;
    this->unk_com = false;
    int fd_rec = invite_check(this->clients[i].cmd[1], this->clients[i].cmd[2], this->clients[i].get_fd_client());
    if (fd_rec == -1)
        return ;
    msg =  this->clients[i].cmd[1] + " :" + this->clients[i].cmd[2] + "\r\n";
    int ch = this->found_channel(this->clients[i].cmd[2]);
    this->channels[ch].invited.push_back(this->clients[i].get_fd_client());
    get_response_name(msg,i, fd_rec);    
}