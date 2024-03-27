/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:27:32 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/26 17:13:59 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

int  check_mode(std::string cmd)
{
    if (cmd[0] == '+' || cmd[0] == '-')
        return (1);
    else
        return (0);
}

void Server::Ch_modes(int i)
{
    std::string msg = ":ircserver 461 MODE :Not enough parameters\r\n";
    if(this->clients[i].cmd.size() < 2)
    {
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    int ch_index = found_channel(this->clients[i].cmd[1]);
    if (ch_index == -1)
    {
        send(this->clients[i].get_fd_client(), ":ircserver 403 MODE :No such channel\r\n", 33, 0);
        return ;
    }
    if (!this->channels[ch_index].is_operator(this->clients[i].get_fd_client()))
    {    
        msg = ":ircserver 482 " + this->channels[ch_index].get_name_channel() + " :You're not channel operator\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    size_t j = check_mode(this->clients[i].cmd[2]);
    for (; j < this->clients[i].cmd[2].size(); j++)
    {
        std::cout << this->clients[i].cmd[2][j] << std::endl;
        msg.clear();
        int param = 3;
        msg = ":ircserver 501 :Unknown MODE flag\r\n";
        if (this->clients[i].cmd[2][j] == 'i')
        {
            this->channels[ch_index].mode += this->clients[i].cmd[2][j];
            msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " i\r\n";
            param--;
        }
        else if (this->clients[i].cmd[2][j] == 't')
        {
            this->channels[ch_index].mode += this->clients[i].cmd[2][j];
            msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " t\r\n";
            param--;
        }   
        else if (this->clients[i].cmd[2][j] == 'k' && !this->clients[i].cmd[param].empty() && this->clients[i].cmd[param].size() <= 8)
        {
            this->channels[ch_index].mode += this->clients[i].cmd[2][j];
            this->channels[ch_index].password = this->clients[i].cmd[param];
            msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + "k " + this->clients[i].cmd[param] + "\r\n";
        }
        else if (this->clients[i].cmd[2][j] == 'l' && !this->clients[i].cmd[param].empty())
        {
            msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " l " + this->clients[i].cmd[param] +" \r\n";
            this->channels[ch_index].max_clients = std::atol(this->clients[i].cmd[param].c_str());
        }
        else if (this->clients[i].cmd[2][j] == 'o' && !this->clients[i].cmd[param].empty())
        {
            if (check_client_channel(this->clients[i].cmd[param],ch_index, 0) < 0)
                msg = ":ircserver 441 " + this->clients[i].cmd[param] + " " + this->channels[ch_index].get_name_channel() + ":No such nick/channel\r\n";
            else
            {
                this->channels[ch_index].operat.push_back(this->client_name[this->clients[i].cmd[param]]);
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " +o" + this->clients[i].cmd[param] + "\r\n";
            }
        }
        else
            break;
        param++;
    } // loop for modes
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    this->clients[i].cmd.clear();
}