/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:29:40 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/30 18:07:59 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server::private_message(int i)
{
    this->unk_com = false;
    split_target(this->clients[i].cmd[1], i, 1);
    if (this->clients[i].split_targ.size() == 0)
    {
        send(this->clients[i].get_fd_client(),  "ircserv 411 :No recipient given PRIVMSG\r\n", 41, 0);
            return;
    }
    for(size_t j = 0 ; j < this->clients[i].split_targ.size() ; j++)
    {
        if (this->clients[i].split_targ[j][0] == '#')
            priv_msg_chan(i, j);
        else
            priv_msg_user(i, j);
    }
    this->clients[i].split_targ.clear();
}

void Server::priv_msg_chan(int i, int j)
{
    std::string msg = "";
    for (size_t k = 0; k < this->channels.size(); k++)
    {
        if (this->channels[k].get_name_channel() == this->clients[i].split_targ[j])
        {
            if (check_client_channel(this->clients[i].get_nickname(), k, 0,0) == -1)
            {
                CannotendToChan(i, j);
                return ;
            }
            for (size_t c = 0; c < this->channels[k]._Client.size(); c++)
            {
                if ((this->channels[k]._Client[c].get_fd_client() != this->clients[i].get_fd_client()))
                {
                    get_response_privmsg(msg, this->clients[i].split_targ[j], i, this->channels[k]._Client[c].get_fd_client());
                    if (this->clients[i].cmd[2][0] == ':')
                        all_arg(msg, i);
                    else
                        msg += this->clients[i].cmd[2] + "\r\n";
                    send(this->channels[k]._Client[c].get_fd_client(),  msg.c_str(), msg.size(), 0);
                }
            }
            return ;
        }
    }
    not_found_target_chan(i,j);
}

void    Server::priv_msg_user(int i, int j)
{
    std::string msg;
    int fd_rec = this->client_name[this->clients[i].split_targ[j]];
    if (this->clients[i].cmd[2].empty())
    {
        send(this->clients[i].get_fd_client(),  "No text to send\r\n", 16, 0);
        return ;
    }
    else if (fd_rec == 0)
        not_found_target_msg(i, j,0);
    else
    {
        get_response_privmsg(msg, this->clients[i].split_targ[j], i, fd_rec);
        if (this->clients[i].cmd[2][0] == ':')
            all_arg(msg, i);
        else
             msg += this->clients[i].cmd[2] + "\r\n";
        send(fd_rec,  msg.c_str(), msg.size(), 0);
    }   
}

void Server::all_arg(std::string &msg, int i)
{
    if (this->clients[i].cmd[2][0] == ':' && this->clients[i].cmd[2][1] == ':')
            this->clients[i].cmd[2].erase(0,1);
    for (size_t k = 2; k < this->clients[i].cmd.size() ; k++)
    {
        msg += this->clients[i].cmd[k];
        if ((k + 1) != this->clients[i].cmd.size())
            msg += " ";
    }
    msg += "\r\n";
}