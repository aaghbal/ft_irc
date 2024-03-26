/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:29:40 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/26 16:48:43 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server::private_message(int i)
{
    this->unk_com = false;
    split_target(this->clients[i].cmd[1], i);
    if (this->clients[i].split_targ.size() == 0)
    {
        send(this->clients[i].get_fd_client(),  "ircserv 411 :No recipient given PRIVMSG\r\n", 42, 0);
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
    for (size_t k = 0; k < this->channels.size(); k++)
    {
        if (this->channels[k].get_name_channel() == this->clients[i].split_targ[j])
        {
            for (size_t c = 0; c < this->channels[k]._Client.size(); c++)
            {
                if ((this->channels[k]._Client[c].get_fd_client() != this->clients[i].get_fd_client()))
                {
                    get_response_name(this->clients[i].split_targ[j], i, this->channels[k]._Client[c].get_fd_client());
                    if (this->clients[i].cmd[2][0] == ':')
                        send_all_arg(i, this->channels[k]._Client[c].get_fd_client());
                    else
                        send(this->channels[k]._Client[c].get_fd_client(),  this->clients[i].cmd[2].c_str(),  this->clients[i].cmd[2].size(), 0);
                    send(this->channels[k]._Client[c].get_fd_client(),  "\r\n", 1, 0);
                }
            }
            return ;
        }
    }
    not_found_target_msg(i,j,0);
}

void    Server::priv_msg_user(int i, int j)
{
    int fd_rec = this->client_name[this->clients[i].split_targ[j]];
    if (this->clients[i].cmd[2].empty())
    {
        send(this->clients[i].get_fd_client(),  "No text to send\r\n", 16, 0);
        return ;
    }
    else if (fd_rec == 0)
        not_found_target_msg(i, j, 1);
    else if (fd_rec != this->clients[i].get_fd_client())
    {
        get_response_name(this->clients[i].split_targ[j], i, fd_rec);
        if (this->clients[i].cmd[2][0] == ':')
            send_all_arg(i, fd_rec);
        else
            send(fd_rec,  this->clients[i].cmd[2].c_str(),  this->clients[i].cmd[2].size(), 0);
        send(fd_rec, "\r\n", 2, 0);
    }   
}

void Server::send_all_arg(int i, int fd_rec)
{
    for (size_t k = 2; k < this->clients[i].cmd.size() ; k++)
    {
        send(fd_rec,  this->clients[i].cmd[k].c_str(),  this->clients[i].cmd[k].size(), 0);
        if ((k + 1) != this->clients[i].cmd.size())
            send(fd_rec,  " ", 1, 0);
    }
}