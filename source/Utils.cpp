/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 12:52:59 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/29 17:05:33 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server::erase_client_from_cha(int i, int num_ch)
{
    split_target(this->clients[i].cmd[2], i, 1);
    for (size_t j = 0; j < this->clients[i].split_targ.size() ; j++)
    {
        if (check_client_channel(this->clients[i].split_targ[j], num_ch, 1,i) == false)
        {
            send(this->clients[i].get_fd_client(), "ircserver 441 ", 14, 0);
            send(this->clients[i].get_fd_client(), this->clients[i].split_targ[j].c_str(), this->clients[i].split_targ[j].size(), 0);
            send(this->clients[i].get_fd_client(), " ", 1, 0);
            send(this->clients[i].get_fd_client(), this->clients[i].cmd[1].c_str(), this->clients[i].cmd[1].size(), 0);
            send(this->clients[i].get_fd_client(), " :They aren't on that channel\r\n", 30, 0); 
        }
    }
}


int Server::found_channel(std::string const &chan)
{
    for(size_t i = 0; i < this->channels.size(); i++)
    {
        if (this->channels[i].get_name_channel() == chan)
            return (i);           
    }
    return (-1);
}

void Server::not_found_target_msg(int i, int j, int fla)
{
    send(this->clients[i].get_fd_client(),  "ircserv 401 ", 13, 0);
    send(this->clients[i].get_fd_client(), this->clients[i].split_targ[j].c_str() , this->clients[i].split_targ[j].size(), 0);
    if (fla)
        send(this->clients[i].get_fd_client(),  " :No such nick\r\n", 15, 0);
    else
        send(this->clients[i].get_fd_client(),  " :No such channel\r\n", 18, 0);
        
}


void Server::not_found_target_chan(int i, int k)
{
    std::string info = this->clients[i].get_nickname() + " " + this->clients[i].split_targ[k];
    std::string msg = ":ircserver 403 " + info + " :No such channel\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    info.clear();
    msg.clear();
}

bool Server::check_client_name(std::string name)
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].get_nickname() == name)
            return true;
    }
    return false;
}

int Server::check_client_channel(std::string name,int ch_index, int flag, int kicker)
{
    for (size_t i = 0; i < this->channels[ch_index]._Client.size(); i++)
    {
        if (this->channels[ch_index]._Client[i].get_nickname() == name)
        {
            if (flag)
                {
                    this->channels[ch_index].Kiked_Client.push_back(this->channels[ch_index]._Client[i].get_nickname());
                    this->channels[ch_index]._Client.erase(channels[ch_index]._Client.begin() + i);
                    std::string msg;
                    if(this->clients[kicker].cmd.size() == 4)
                        msg = ":" + this->clients[kicker].get_nickname() + "!" + this->clients[i].get_username() + "@" "localhost " + " KICK " + this->channels[ch_index].get_name_channel() + " " + name +' '+ this->clients[kicker].cmd[3] + "\r\n";
                    else
                        msg = ":" + this->clients[kicker].get_nickname() + "!" + this->clients[i].get_username() + "@" "localhost " + " KICK " + this->channels[ch_index].get_name_channel() + " " + name + " :Kicked by " + this->clients[kicker].get_nickname() + "\r\n";
                    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
                }
            return i;
        }
    }
    return -1;
}

void Server::disconnect_client(int i)
{
    this->unk_com = false;
    close(this->clients[i].get_fd_client());
    //this->clients[i - 1].cmd.clear();// segfault
    this->polfd.erase(polfd.begin() + i + 1);
    this->clients.erase(this->clients.begin() + i);
    std::cout << "this client " << i + 1 << " closed" << std::endl;
    // if (this->channels.size() != 0)
    //     this->channels.erase(this->channels.begin() + i - 1);
}

