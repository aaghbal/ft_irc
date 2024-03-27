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
//Error(401): No such nick/channelError(401): No such nick/channel 06:07
int Server::invite_check(std::string &nick, std::string &chan, int fd)
{
    std::string msg;
    if (nick.empty() || chan.empty())
        return (-1);
    int fd_rec = this->client_name[nick]; // here we get the fd of the client we want to invite ERROR here Fd is not right
    std::cout <<"chammmmmel" << chan << std::endl;
    int ch = found_channel(chan);
    if(fd_rec == 0 || ch == -1)
    {
        std::cout << fd_rec << " " << ch << std::endl;
        std::string msg = ":ircserver 401 " + nick + " :No such nick/channel" ;
        msg+= LF;
        send(fd, msg.c_str(), msg.size(), 0);
        return (-1);
    }
    if (check_client_channel(nick, ch, 0))
    {
        msg = ":ircserver 443 " + this->clients[fd].get_nickname() + " " + nick + " " + chan + " :is already on channel" + LF;
        send(fd, msg.c_str(), msg.size(), 0);
        return (-1);
    }
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