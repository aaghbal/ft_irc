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

bool password_channels(std::string &pass)
{
    if (pass.size() < 8 || pass.size() > 20)
        return false;
    return true;
}
std::string set_mode(std::string &mode,char m)
{
    std::string mod;
    for (size_t i = 0; i < mode.size(); i++)
    {
        if (mode[i] == m)
            continue;
        mod += mode[i];
    }
    return mod;
}

void Server::Ch_modes(int i)
{

    std::string msg = ":ircserver 461 MODE :Not enough parameters\r\n";
    if(this->clients[i].cmd.size() < 3)
        return ;
    int ch_index = found_channel(this->clients[i].cmd[1]);
    if (ch_index == -1)
    {
        send(this->clients[i].get_fd_client(), ":ircserver 403 MODE :No such channel\r\n", 33, 0);
        return ;
    }
    if (!this->channels[ch_index].is_operator(this->clients[i].get_fd_client()))
    {    
        msg = ":ircserver 482 " + this->channels[ch_index].get_name_channel() + " :You're not channel operator 88\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    size_t j = check_mode(this->clients[i].cmd[2]);
    for (size_t param = 3; j < this->clients[i].cmd[2].size(); j++)
    {
        msg.clear();
        msg = ":ircserver 501 MODE :Unknown MODE flag\r\n";
        if (this->clients[i].cmd[2][j] == 's' || this->clients[i].cmd[2][j] == 'p' || this->clients[i].cmd[2][j] == 'n' || this->clients[i].cmd[2][j] == 'm' || this->clients[i].cmd[2][j] == 'q' || this->clients[i].cmd[2][j] == 'a' || this->clients[i].cmd[2][j] == 'h' || this->clients[i].cmd[2][j] == 'v')
            return;
        else if (this->clients[i].cmd[2][j] == 'i')
        {
            if(this->clients[i].cmd[2][0] == '-')
            {
                this->channels[ch_index].mode = set_mode(this->channels[ch_index].mode, 'i');
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " -i\r\n";
            }
            else
            {
                this->channels[ch_index].mode += this->clients[i].cmd[2][j];
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " +i\r\n";
            }
            param--;
        }
        else if (this->clients[i].cmd[2][j] == 't')
        {
            if(this->clients[i].cmd[2][0] == '-')
            {
                this->channels[ch_index].mode = set_mode(this->channels[ch_index].mode, 't');
                this->channels[ch_index].topic = "No topic is set.";
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " -t\r\n";
                continue; 
            }
            if (this->clients[i].cmd.size() < param)
            {
                msg = ":ircserver 461 MODE :Not enough parameters\r\n";
                break;
            }
            else
            {
                this->channels[ch_index].set_topic(this->clients[i].cmd[param]);
                this->channels[ch_index].mode += this->clients[i].cmd[2][j];
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " t\r\n";
            }   
            param--;
        }   
        else if (this->clients[i].cmd[2][j] == 'k')
        {
            this->channels[ch_index].mode += this->clients[i].cmd[2][j];
            if(this->clients[i].cmd[2][0] == '-')
            {
                this->channels[ch_index].mode = set_mode(this->channels[ch_index].mode, 'k');
                this->channels[ch_index].password.clear();
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " -k\r\n";
                continue;
            }
            if (this->clients[i].cmd.size() < param)
            {
                msg = ":ircserver 461 MODE :Not enough parameters\r\n";
                break;
            }
            else if (!password_channels(this->clients[i].cmd[param]))
            {
                msg = ":ircserver 475 " + this->channels[ch_index].get_name_channel() + " :Bad Channel Key\r\n";
                break;
            }
            else
            {
                this->channels[ch_index].password = this->clients[i].cmd[param];
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + "+k " + this->clients[i].cmd[param] + "\r\n";
            }
        }
        else if (this->clients[i].cmd[2][j] == 'l')
        {
            if(this->clients[i].cmd[2][0] == '-')
            {
                this->channels[ch_index].mode = set_mode(this->channels[ch_index].mode, 'l');
                this->channels[ch_index].max_clients = 1000;
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " -l\r\n";
                continue;
            }
            if (this->clients[i].cmd.size() < param)
            {
                msg = ":ircserver 461 MODE :Not enough parameters\r\n";
                break;
            }
            size_t max_cl = std::atol(this->clients[i].cmd[param].c_str());
            if(this->channels[ch_index]._Client.size() > max_cl)
            {
                msg = ":ircserver 471 " + this->channels[ch_index].get_name_channel() + " :Channel has allready more Clients \r\n";
                break;
            }
            else if(max_cl > 1000 || max_cl < 3)
            {
                msg = ":ircserver 477 " + this->channels[ch_index].get_name_channel() + " :Channel Max client Can Take only less then 1000 and Greater than 3\r\n";
                break;
            }
            this->channels[ch_index].max_clients = max_cl;
            this->channels[ch_index].mode += 'l';
            msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " +l " + this->clients[i].cmd[param] + "\r\n";
        }
        else if (this->clients[i].cmd[2][j] == 'o')
        {
            if (check_client_channel(this->clients[i].cmd[param],ch_index, 0,0) == -1)
            {
                msg = ":ircserver 441 " + this->clients[i].cmd[param] + " " + this->channels[ch_index].get_name_channel() + ":No such nick/channel\r\n";
                break;
            }
            if(this->clients[i].cmd[2][0] == '-')
            {
                this->channels[ch_index].mode = set_mode(this->channels[ch_index].mode, 'o');
                this->channels[ch_index].erase_operat(this->client_name[this->clients[i].cmd[param]]);
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " -o\r\n";
                continue;
            }
            if (this->clients[i].cmd.size() < param)
            {
                msg = ":ircserver 461 MODE :Not enough parameters\r\n";
                break;
            }
            else
            {
                this->channels[ch_index].operat.push_back(this->client_name[this->clients[i].cmd[param]]);
                msg = ":ircserver MODE " + this->channels[ch_index].get_name_channel() + " +o" + this->clients[i].cmd[param] + "\r\n";
            }
        }
        else
            break;
        param++;
    }
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    this->clients[i].cmd.clear();
}