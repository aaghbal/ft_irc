/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:57:26 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/31 15:54:24 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"


void Server::join_cmd(int i) //pasword check needed
{
    bool mod = false;
    this->unk_com = false;
    std::string msg;
    if(this->clients[i].cmd.size() < 2)
    {
        msg = ":IRCsERVER 461 " + this->clients[i].get_nickname() + " JOIN :Not enough parameters\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        msg.clear();
        return ;
    }
    split_target(this->clients[i].cmd[1], i, 1);
    if(this->clients[i].cmd.size() == 3)
        split_target(this->clients[i].cmd[2], i, 0);
    for (size_t k = 0; k < this->clients[i].split_targ.size(); k++)
    {
        int n_ch = this->found_channel(this->clients[i].split_targ[k]);
        if (this->clients[i].split_targ[k][0] != '#')
            not_found_target_chan(i, k);
        else if (n_ch == -1)
            create_new_chan(i, k);
        else if (check_client_channel(this->clients[i].cmd[1], n_ch, 0,0) != -1)
        {
            std::string info = this->clients[i].get_nickname() + " " + this->channels[n_ch].get_name_channel();
            msg = ":IRCsERVER 443 " + info + " :is already on channel\r\n";
            send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
            info.clear();
            msg.clear();
        }
        else
            mod = check_mode_chan(n_ch, i);
        if (mod)
            join_channel(n_ch, i, k);
    }
    this->clients[i].split_targ.clear();
}

void Server::create_new_chan(int i, int k)
{
    Channel ch;
    ch.set_name(this->clients[i].split_targ[k]);
    ch._Client.push_back(this->clients[i]);
    ch.topic = "No topic is set";
    ch.operat.push_back(this->clients[i].get_fd_client());
    ch.max_clients = 1000;
    if (this->clients[i].split_targ[k].size() == 1)
    {
        ErrBadChannelKey(i, k);
        return ;
    }
    this->channels.push_back(ch);
    joined_message(this->clients[i].get_fd_client(), i, -1, k); 
}

bool Server::check_mode_chan(int n_ch, int i)
{
    std::string info = this->clients[i].get_nickname() + " " + this->channels[n_ch].get_name_channel();
    std::string msg = "";
    if (this->channels[n_ch].mode.find('i') != std::string::npos && !this->channels[n_ch].is_invited(this->clients[i].get_fd_client()))
    {
        msg = ":IRCsERVER 473 " + info + " :Cannot join channel (+i)\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        info.clear();
        msg.clear();
        return false;
    }
    else if (this->channels[n_ch].mode.find('k') != std::string::npos && this->clients[i].cmd.size() < 3)
    {
        msg = ":IRCsERVER 475 " + info + " :Cannot join channel (+k)\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        info.clear();
        msg.clear();
        return false ;
    }
    else if (this->channels[n_ch].mode.find('l') != std::string::npos && this->channels[n_ch]._Client.size() == this->channels[n_ch].max_clients)
    {
        msg = ":IRCsERVER 471 " + info + " :Cannot join channel (+l)\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        info.clear();
        msg.clear();
        return false ;
    }
    else if (this->channels[n_ch].joined_in_channel(this->clients[i].get_fd_client()))
    {
        msg = ":IRCsERVER 443 " + info + " :is already on channel\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        info.clear();
        msg.clear();
        return false ;
    }
    info.clear();
    return true ;
}

void Server::join_channel(int n_ch, int i, int k)
{
    if (this->channels[n_ch].mode.find('k') != std::string::npos)
    {
        std::string info = this->clients[i].get_nickname() + " " + this->channels[n_ch].get_name_channel();
        std::string msg = ":IRCsERVER 475 " + info + " :Cannot join channel (+k) - bad key\r\n";
        if (this->channels[n_ch].password == this->clients[i].cmd[2])
        {
            this->channels[n_ch]._Client.push_back(this->clients[i]);
            joined_message(this->clients[i].get_fd_client(), i, n_ch, k);
        }
        else
            send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        info.clear();
        msg.clear();
        return ;
    }
    if(this->channels[n_ch].mode.find('l') != std::string::npos)
        if(this->channels[n_ch].max_clients == this->channels[n_ch]._Client.size())
        {
            std::string info = this->clients[i].get_nickname() + " " + this->channels[n_ch].get_name_channel();
            std::string msg = ":ircserver 471 " + info + " :Cannot join channel (+l)\r\n";
            send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
            info.clear();
            msg.clear();
            return ;
        }
    if(this->channels[n_ch].Kiked_Client.size() > 0)
        for(size_t j = 0; j < this->channels[n_ch].Kiked_Client.size(); j++)
            if(this->channels[n_ch].Kiked_Client[j] == this->clients[i].get_nickname())
            {
                std::string info = this->clients[i].get_nickname() + " " + this->channels[n_ch].get_name_channel();
                std::string msg = ":ircserver 474 " + info + " :Cannot join channel Kiked\r\n";
                send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
                info.clear();
                msg.clear();
                return ;
            }
    this->channels[n_ch]._Client.push_back(this->clients[i]);
    joined_message(this->clients[i].get_fd_client(), i, n_ch, k);
}


void Server::joined_message(int fd, int i, int cha, int k)
{
    std::string str = "";
    if (cha == -1)
    {
        get_response_join(clients[i].split_targ[k], i, fd);
        str = ":IRCsERVER 353 " + clients[i].get_nickname() + " = " + clients[i].split_targ[k] + " :@" + clients[i].get_nickname() + "\r\n";
        send(fd, str.c_str(), str.size(), 0);
        str.clear();
    }
    else
    {
        for(size_t j = 0; j < channels[cha]._Client.size(); j++)
        {
            get_response_join(clients[i].split_targ[k], i, channels[cha]._Client[j].get_fd_client());
        }
        str = ":IRCsERVER 353 " + clients[i].get_nickname() + " = " + clients[i].split_targ[k] + " :";
        GetUserChannel(str, cha);

        send(fd, str.c_str(), str.size(), 0);
        str.clear();
    }
    str = ":IRCsERVER 366 " + clients[i].get_nickname() + " " + clients[i].split_targ[k] + " :End of /NAMES list.\r\n";
    send(fd, str.c_str(), str.size(), 0);
    str.clear();
}
