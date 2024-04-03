/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authenticate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:39:32 by aaghbal           #+#    #+#             */
/*   Updated: 2024/04/03 18:11:34 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"


void Server::check_password(int j)
{
    if (this->clients[j].cmd[0] == "PASS")
    {
        if (this->clients[j].cmd.size() == 1)
            Err_NeedMoreParam(j);
        else if (this->password == this->clients[j].cmd[1])
            this->clients[j].authenticate = true;
        else
        {
            std::string rec_mes = ":IRCsERVER 464 " + this->clients[j].cmd[1] + INCCORRECT_PASS;
            send(this->clients[j].get_fd_client(), rec_mes.c_str() , rec_mes.size(), 0);
        }
    }
    else if (this->clients[j].cmd[0] == "USER" ||this->clients[j].cmd[0] == "NICK")
        Err_NotRegistered(j);
    else
    {
        std::string msg = ":IRCsERVER 421" + this->clients[j].get_nickname() + " " + this->clients[j].cmd[0];
        msg += " :Unknown command\r\n";
        send(this->clients[j].get_fd_client(), msg.c_str(), msg.size(), 0);
        msg.clear();
    }
}

void Server::check_nickname(int j)
{
    if (this->clients[j].cmd[0] == "NICK" && this->clients[j].authenticate)
    {
        if (this->clients[j].authenticate == false)
            Err_NotRegistered(j);
        else if (this->clients[j].cmd.size() == 1)
            Err_NeedMoreParam(j);
        else if (parsing_nickname(j))
            return ;
        else if (this->check_client_name(this->clients[j].cmd[1]))
        {
            std::string rec_mes = ":IRCsERVER 433 " + this->clients[j].cmd[1] + NICKNAME_IN_USE;
            send(this->clients[j].get_fd_client(), rec_mes.c_str(), rec_mes.size(), 0);
        }
        else
        {
            this->clients[j].set_nickname(this->clients[j].cmd[1]);
            this->client_name[this->clients[j].cmd[1]] = this->clients[j].get_fd_client();
            this->clients[j].nick_succ = true;
        }
    }
    else
    {
        std::string msg = ":IRCsERVER 421 " + this->clients[j].get_nickname() + " " + this->clients[j].cmd[0];
        msg += " :Unknown command\r\n";
        send(this->clients[j].get_fd_client(), msg.c_str(), msg.size(), 0);
        msg.clear();
    }
}

void Server::check_username(int j)
{
    if (this->clients[j].nick_succ == false)
        send(this->clients[j].get_fd_client(), "Enter the nickname first\r\n", 26, 0);
    else if(this->clients[j].cmd[0] == "USER") 
    {
        if (this->clients[j].authenticate == false)
            Err_NotRegistered(j);
        else if (this->clients[j].cmd.size() == 1)
            Err_NeedMoreParam(j);
        else
        {
            this->clients[j].set_username(this->clients[j].cmd[1]);
            this->clients[j].reg_end = true;
            Command_Responses(j);
        }
    }
    else
    {
        std::string msg = ":IRCsERVER 421 " + this->clients[j].get_nickname() + " " + this->clients[j].cmd[0];
        msg += " :Unknown command\r\n";
        send(this->clients[j].get_fd_client(), msg.c_str(), msg.size(), 0);
        msg.clear();
    }
}

void Server::authenticate(int j)
{
    if (this->clients[j].authenticate == false)
    {
        check_password(j);
        return ;
    }
    switch (this->clients[j].cmd[0][0])
    {
        case 'N':
           check_nickname(j);
           break;
        case 'U':
           check_username(j);
           break;
        default:
           return ;
    }
}
int Server::get_indx_channel(std::string name)
{
    for (size_t i = 0; i < this->channels.size(); i++)
    {
        if(check_client_channel(name ,i,0,0) != -1)
            return i;
    }
    return -1;
}

void    Server::change_nikname(int i)
{
    this->unk_com = false;
    if (this->clients[i].cmd.size() != 2 || (this->clients[i].cmd.size() == 2 && this->clients[i].cmd[1] == ":"))
        No_NicknameGiven(i);
    else if (parsing_nickname(i))
        return ;
    else if (this->check_client_name(this->clients[i].cmd[1]))
    {
        std::string rec_mes = ":IRCsERVER 433 " + this->clients[i].cmd[1] + NICKNAME_IN_USE;
        send(this->clients[i].get_fd_client(), rec_mes.c_str(), rec_mes.size(), 0);
    }
    else
    {
        get_response_nick(this->clients[i].cmd[1], i, this->clients[i].get_fd_client());
        std::cout << this->clients[i].cha_join.size() << std::endl;
        for (size_t c = 0; c < this->clients[i].cha_join.size(); c++)
        {
            int ch_cl_fd = check_client_channel(this->clients[i].get_nickname(),this->clients[i].cha_join[c],0,0);
            this->channels[this->clients[i].cha_join[c]]._Client[ch_cl_fd].set_nickname(this->clients[i].cmd[1]);
        }
        this->client_name[this->clients[i].cmd[1]] = this->clients[i].get_fd_client();
        this->clients[i].set_nickname(this->clients[i].cmd[1]);
    }
}