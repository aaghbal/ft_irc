/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authenticate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:39:32 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/26 16:45:39 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"


void Server::check_password(int j)
{
    if (this->clients[j].cmd[0] == "PASS" || this->clients[j].cmd[0] == "pass")
    {
        if (this->password == this->clients[j].cmd[1])
            this->clients[j].authenticate = true;
        else
        {
            std::string rec_mes = ":ircserver 464 " + this->clients[j].cmd[1] + INCCORRECT_PASS;
            send(this->clients[j].get_fd_client(), rec_mes.c_str() , rec_mes.size(), 0);
        }
    }
    else
    {
        std::string msg = ":ircserver 421 " + this->clients[j].get_nickname() + " " + this->clients[j].cmd[0];
        msg += " :Unknown command\r\n";
        send(this->clients[j].get_fd_client(), msg.c_str(), msg.size(), 0);
        msg.clear();
    }
}

void Server::check_nickname(int j)
{
    if ((this->clients[j].cmd[0] == "NICK" || this->clients[j].cmd[0] == "nick")  && this->clients[j].authenticate)
    {
        if (this->check_client_name(this->clients[j].cmd[1]))
        {
            std::string rec_mes = ":ircserver 433 " + this->clients[j].cmd[1] + NICKNAME_IN_USE;
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
        std::string msg = ":ircserver 421 " + this->clients[j].get_nickname() + " " + this->clients[j].cmd[0];
        msg += " :Unknown command\r\n";
        send(this->clients[j].get_fd_client(), msg.c_str(), msg.size(), 0);
        msg.clear();
    }
}

void Server::check_username(int j)
{
    if (this->clients[j].nick_succ == false)
    {
        send(this->clients[j].get_fd_client(), "Enter the nickname first\r\n", 26, 0);
        return ;
    }
    if((this->clients[j].cmd[0] == "USER" || this->clients[j].cmd[0] == "user")) 
    {
        this->clients[j].set_username(this->clients[j].cmd[1]);
        this->clients[j].reg_end = true;
        std::string rec_mes = ":ircserver 001 " + this->clients[j].cmd[1] + WELCOME_MSG;
        send(this->clients[j].get_fd_client(), rec_mes.c_str(), rec_mes.size(), 0);
        rec_mes.clear();
    }
    else
    {
        std::string msg = ":ircserver 421 " + this->clients[j].get_nickname() + " " + this->clients[j].cmd[0];
        msg += " :Unknown command\r\n";
        send(this->clients[j].get_fd_client(), msg.c_str(), msg.size(), 0);
        msg.clear();
    }
}

void Server::authenticate(int j)
{
    if (this->clients[j].cmd.size() != 2 && this->clients[j].cmd.size() != 5)
    {
        send(this->clients[j].get_fd_client(), "Error parameters\r\n", 18, 0);
        this->clients[j].cmd.clear();
        return ;
    }
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
        case 'n':
           check_nickname(j);
           break;
        case 'U':
           check_username(j);
           break;
        case 'u':
           check_username(j);
           break;
        default:
           return ;
    }
}

void    Server::change_nikname(int i)
{
    this->unk_com = false;
    if (this->check_client_name(this->clients[i].cmd[1]))
    {
        std::string rec_mes = ":ircserver 433 " + this->clients[i].cmd[1] + NICKNAME_IN_USE;
        send(this->clients[i].get_fd_client(), rec_mes.c_str(), rec_mes.size(), 0);
    }
    else
    {
        get_response_name(this->clients[i].cmd[1], i, this->clients[i].get_fd_client());
        this->clients[i].set_nickname(this->clients[i].cmd[1]);
        this->client_name[this->clients[i].cmd[1]] = this->clients[i].get_fd_client();
    }
}