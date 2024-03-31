/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:40:36 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/31 15:54:24 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server::kick_command(int i)
{
    std::string msg;
    this->unk_com = false;
    int num_ch = found_channel(this->clients[i].cmd[1]);
    if (num_ch == -1)
    {
        msg = ":IRCsERVER 403 " + this->clients[i].cmd[1] + " : No such channel" + LF;
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    if (this->channels[num_ch].joined_in_channel(this->clients[i].get_fd_client()) == false)
    {
        msg = ":IRCsERVER 441 " + this->clients[i].cmd[1] + " " + this->channels[num_ch].get_name_channel() + " :You're not on that channel" + LF;
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }//:irc.example.com 482 <channel> :You're not channel operator
    if (this->channels[num_ch].is_operator(this->clients[i].get_fd_client()) == false)
    {
        msg = ":IRCsERVER 482 " + this->channels[num_ch].get_name_channel() + " :You're not channel operator" + LF;
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    erase_client_from_cha(i, num_ch);
    this->clients[i].split_targ.clear();
}