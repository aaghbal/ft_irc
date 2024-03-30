/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:27:32 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/30 21:16:16 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server::Ch_modes(int i)
{
    this->unk_com = false;
    std::string allowed_modes = "itkol";
    int ch_index = found_channel(this->clients[i].cmd[1]);
    if (this->clients[i].cmd.size() < 3)
        return ;
    if (ch_index == -1)
    {
        send(this->clients[i].get_fd_client(), ":ircserver 403 :No such channel\r\n", 33, 0);
        return ;
    }
    if((this->clients[i].cmd[2][0] == '+' || this->clients[i].cmd[2][0] == '-' ) && (allowed_modes.find(this->clients[i].cmd[2][1]) != std::string::npos))
    {
        std::string msg = ":ircserver 461 MODE :Not enough parameters\r\n";
        if (this->clients[i].cmd[2][1] == 'i' && this->clients[i].cmd[3].empty())
        {
            this->channels[ch_index].mode += this->clients[i].cmd[2][1];
            return ;
        }
        else if (this->clients[i].cmd[2][1] == 't' && this->clients[i].cmd[3].empty())
        {
            this->channels[ch_index].mode += this->clients[i].cmd[2][1];
            return ;
        }   
        else if (this->clients[i].cmd[2][1] == 'k' && !this->clients[i].cmd[3].empty() && this->clients[i].cmd[3].size() <= 8)
        {
            std::cout << "cmd : " << this->clients[i].cmd[3] << std::endl;
            this->channels[ch_index].mode += this->clients[i].cmd[2][1];
            this->channels[ch_index].password = this->clients[i].cmd[3];
            return ;
        }
        else if (this->clients[i].cmd[2][1] == 'l' && !this->clients[i].cmd[3].empty())
        {
            this->channels[ch_index].max_clients = std::atol(this->clients[i].cmd[3].c_str());//check if the number is valid
            return ;
        }
        else if (this->clients[i].cmd[2][1] == 'o' && !this->clients[i].cmd[3].empty())
        {
            if (!this->channels[ch_index].is_operator(this->clients[i].get_fd_client()))
                msg = ":ircserver 482 " + this->channels[ch_index].get_name_channel() + " :You're not channel operator\r\n";
            else
                msg = ":ircserver 441 " + this->clients[i].cmd[3] + " " + this->channels[ch_index].get_name_channel() + ":No such nick/channel\r\n";
            if (check_client_channel(this->clients[i].cmd[3],ch_index, 0) != -1)
            {
                this->channels[ch_index].operat.push_back(this->client_name[this->clients[i].cmd[3]]);
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " +o" + this->clients[i].cmd[3] + "\r\n";
            }
        }
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    }
    // else
    // {
    //     send(this->clients[i].get_fd_client(), ":ircserver 501 :Unknown MODE flag\r\n", 47, 0);
    //     return ;
    // }
}