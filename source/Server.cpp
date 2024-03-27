/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:47:14 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/26 17:08:50 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

Server::Server()
{
    this->client_info.clear();
    this->unk_com = true;
}

void Server::set_port(const char *port)
{
    this->port = port;
}

void Server::set_password(const std::string &password)
{
    this->password = password;
}

void Server::init_sockinfo(void)        
{
    this->sockinfo.sin_family= AF_INET;
    this->sockinfo.sin_port = htons(std::atoi(this->port));
    this->sockinfo.sin_addr.s_addr = INADDR_ANY;
}

void Server::create_socket(void)
{
    try
    {
        this->fd_s = socket(this->sockinfo.sin_family,  SOCK_STREAM, 0);
        if (this->fd_s == -1)
            throw Error();
        if (fcntl(this->fd_s, F_SETFL, O_NONBLOCK) == -1)
            throw Error();
    }
    catch(const Error& e)
    {
        e.ErrorSocket();
    }
}

void Server::bind_socket(void)
{
       try
    {
        int yes = 1;
        if (setsockopt(this->fd_s,SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes)) == -1)
            std::cout << "errrrrrrooooorrr" << std::endl;
        if (bind(this->fd_s,(const sockaddr *)&this->sockinfo , sizeof(this->sockinfo)) == -1)
            throw Error();
            
    }
    catch(const Error& e)
    {
        e.ErrorBind();
    }  
}

void Server::listen_requ(void)
{ 
    try
    {
        if (listen(this->fd_s, 20) == -1)
            throw Error();
        this->polfd.push_back(init_pollfd(this->fd_s));
    }
    catch(const Error& e)
    {
        e.ErrorListen();
    }   
}

void Server::run_server()
{
    init_sockinfo();
    create_socket();
    bind_socket();
    listen_requ();
    accept_data();
}


void Server::disconnect_client(int i)
{
    this->unk_com = false;
    close(this->clients[i].get_fd_client());
    this->polfd.erase(polfd.begin() + i + 1);
    this->clients.erase(this->clients.begin() + i);
     std::cout << "this client " << i + 1 << " closed" << std::endl;
    // if (this->channels.size() != 0)
    //     this->channels.erase(this->channels.begin() + i - 1);
}

int Server::check_client_channel(std::string name,int ch_index, int flag)
{
    for (size_t i = 0; i < this->channels[ch_index]._Client.size(); i++)
    {
        if (this->channels[ch_index]._Client[i].get_nickname() == name)
        {
            if (flag)
                this->channels[ch_index]._Client.erase(channels[ch_index]._Client.begin() + i);
            return i;
        }
    }
    return -1;
}

void Server::erase_client_from_cha(int i, int num_ch)
{
    split_target(this->clients[i].cmd[2], i);
    for (size_t j = 0; j < this->clients[i].split_targ.size() ; j++)
    {
        if (check_client_channel(this->clients[i].split_targ[j], num_ch, 1) == false)
        {
            send(this->clients[i].get_fd_client(), "ircserver 441 ", 14, 0);
            send(this->clients[i].get_fd_client(), this->clients[i].split_targ[j].c_str(), this->clients[i].split_targ[j].size(), 0);
            send(this->clients[i].get_fd_client(), " ", 1, 0);
            send(this->clients[i].get_fd_client(), this->clients[i].cmd[1].c_str(), this->clients[i].cmd[1].size(), 0);
            send(this->clients[i].get_fd_client(), " :They aren't on that channel\r\n", 30, 0); 
        }
    }
}
void Server::get_response_name(std::string &cmd, int i, int fd)
{
    std::string msg;
    msg += ":" + this->clients[i].get_nickname();
    msg += "!~";
    msg += this->clients[i].get_username();
    msg += '@';
    struct sockaddr_in t = this->client_info[fd];
    msg += inet_ntoa(t.sin_addr);
    if (this->clients[i].cmd[0] == "PRIVMSG")
        msg += " PRIVMSG ";
    else if (this->clients[i].cmd[0] == "JOIN")
        msg += " JOIN ";
    else if (this->clients[i].cmd[0] == "NICK")
        msg += " NICK :";
    else if (this->clients[i].cmd[0] == "INVITE")
        msg += " INVITE ";
    msg += cmd;
    if (this->clients[i].cmd[0] == "NICK")
        msg += "\r\n";
    else
        msg += " ";
    send(fd, msg.c_str(), msg.size(), 0);
}

pollfd Server::init_pollfd(int fd)
{
    struct  pollfd pfd;

    pfd.fd = fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    return pfd;
}

std::vector<std::string> Server::split_cmd(std::string &cmd)
{
    std::vector<std::string>    MySplitData;
    std::string                 token;

    for(size_t i = 0; i < cmd.size() ; i++)
    {
        char c = cmd[i];
        if(c != ' ' && c != '\t' && c != '\n' && c != '\r')
            token += c;
        else if (!token.empty())
        {
            MySplitData.push_back(token);
            token.clear();
        }
    }
    if (!token.empty())
        MySplitData.push_back(token);
    cmd.clear();
    return (MySplitData);
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

void Server::split_target(std::string &cmd, int fd)
{
    std::string token = "";
    for(size_t i = 0; i < cmd.size(); i++)
    {
        char c = cmd[i];
        if(c != ',')
            token += c;
        else if (!token.empty())
        {
           
            this->clients[fd].split_targ.push_back(token);
            token.clear();
        }
    }
    if (!token.empty())
            this->clients[fd].split_targ.push_back(token);
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

bool Server::check_client_name(std::string name)
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].get_nickname() == name)
            return true;
    }
    return false;
}
