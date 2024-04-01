/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:41:58 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/31 14:10:05 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server::InviteCommand(int i)
{
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

