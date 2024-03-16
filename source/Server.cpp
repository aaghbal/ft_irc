/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:47:14 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/16 22:08:06 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

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
        int opt = 1;
        setsockopt(this->fd_s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
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

void Server:: accept_data(void)
{
    while (true)
    {
        std::cout << "polling..." << std::endl;
        int nm = poll(this->polfd.data(), this->polfd.size(), -1);
        if (nm == -1)
            break;
        if (nm == 0)
            continue;
        for(size_t i = 0; i < this->polfd.size(); i++)
        {
            if (this->polfd[i].revents & POLLIN)
            {
                if(this->polfd[i].fd == this->fd_s)
                    add_new_connection();
                else
                    recive_data(i);
            }
        }
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
bool Server::check_recv_message(int i)
{
    if (this->nbyteread <= 0)
    {
        if (this->nbyteread == 0)
            std::cout << "this client " << i << " closed" << std::endl;
        else
            std::cerr << "recv failed" << std::endl; 
        close(this->polfd[i].fd);
        this->polfd.erase(polfd.begin() + i);
        this->clients.erase(this->clients.begin() + i - 1);
        if (this->channels.size() != 0)
            this->channels.erase(this->channels.begin() + i - 1);
        return true;
    }
    return false;
}

void Server::check_password(int i)
{
    if(this->clients[i].cmd.size() != 2 || (this->clients[i].cmd[0] != "PASS"))
    {
        send(this->clients[i].get_fd_client(), "ircserv 461 PASS :Not enough parameters\n", 40, 0);
        return ;
    }
    if (this->password == this->clients[i].cmd[1])
        this->clients[i].authenticate = true;
    else
    {
        if (this->clients[i].num_pass == 2)
        {
            send(this->clients[i].get_fd_client(), "Access denied!! 3 attempts completed.\n", 38, 0);
            close(this->clients[i].get_fd_client());
            this->polfd.erase(polfd.begin() + i + 1);
            this->clients.erase(this->clients.begin() + i);
            return;
        }
        send(this->clients[i].get_fd_client(), "The password is incorrect!, try again\n", 38, 0);
        this->clients[i].num_pass++;
    }
}

void Server::add_new_connection(void)
{
    struct sockaddr_in clinfo;
    this->len = sizeof(clinfo);
    Client   cl;
    try
    {
        int new_fd_s = accept(this->fd_s, (sockaddr *)&clinfo, &len);
        std::cout << "new client with fd : " << new_fd_s << std::endl;
        if (new_fd_s == -1)
            throw Error();
        cl.set_fd_client(new_fd_s);
        this->clients.push_back(cl);
        this->polfd.push_back(init_pollfd(new_fd_s));
        this->client_info.push_back(clinfo);
    }
    catch(const Error& e)
    {
        e.ErrorAccept();
    }

}

int    myRevc(std::string &str , int fd)
{
    char buff[1001];
    int ret = recv(fd, buff, 1000, 0);
    buff[ret - 1] = '\0';
    str = buff;
    return ret;
}

void Server::recive_data(int i)
{
    this->nbyteread = myRevc(this->clients[i - 1].buff, this->clients[i - 1].get_fd_client());
    if (check_recv_message(i))
        return ;
    this->clients[i - 1].cmd = split_cmd(this->clients[i - 1].buff);
    if((this->clients[i - 1].cmd.size() == 0))
    {
        this->clients[i - 1].cmd.clear();
        return ;
    }
    if (this->clients[i - 1].info_client_fin == false)
        init_client(i - 1);
    else
    {
        if ( this->clients[i - 1].cmd[0] == "PRIVMSG")
            private_message(i - 1);
        else if (this->clients[i - 1].cmd[0] == "JOIN")
            this->join_cmd(i - 1);
    }
}

pollfd Server::init_pollfd(int fd)
{
    struct  pollfd pfd;

    pfd.fd = fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    return pfd;
}


void Server::init_client(int i)
{
    if (this->clients[i].authenticate)
    {
        if (this->clients[i].get_nickname().empty())
        {
            if((this->clients[i].cmd.size() != 2 || this->clients[i].cmd[0] != "NICK"))
            {
                send(this->clients[i].get_fd_client(), "Syntax error : NICK <nickname>\n", 32, 0);
                return ;
            }
            if (this->client_name.find(this->clients[i].cmd[1]) != this->client_name.end())
                send(this->clients[i].get_fd_client(), "this nickname had already taken Try another one.\n", 49, 0);
            else
            {
                this->client_name[this->clients[i].cmd[1]] = this->clients[i].get_fd_client();
                this->clients[i].set_nickname(this->clients[i].cmd[1]);
            }
        }
        else if (this->clients[i].get_username().empty())
        {
            if(this->clients[i].cmd.size() != 2 || (this->clients[i].cmd[0] != "USER"))
            {
                send(this->clients[i].get_fd_client(), "Syntax error : USER <username>\n", 32, 0);
                return ;
            }
            this->clients[i].set_username(this->clients[i].buff);
                send(this->clients[i].get_fd_client(),  "Welcome to the server\n", 22, 0);
            this->clients[i].info_client_fin = true;
        }
    }
    if (this->clients[i].authenticate == false)
        check_password(i);
}

std::vector<std::string> Server::split_cmd(std::string &cmd)
{
    std::vector<std::string>    MySplitData;
    std::string                 token;

    std::cout << ">>> " << cmd << std::endl;
    for(size_t i = 0; i < cmd.size() ; i++)
    {
        char c = cmd[i];
        if(c != ' ' && c != '\t')
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

void Server::private_message(int i)
{
    int fd_rec = this->client_name[this->clients[i].cmd[1]];
    if (this->clients[i].cmd[1].empty())
        send(this->clients[i].get_fd_client(),  "\n", 28, 0);
    else if (this->clients[i].cmd[2][0] != ':')
        send(this->clients[i].get_fd_client(),  "The message must begin with < : >\n", 35, 0);
    else if (fd_rec == 0)
        send(this->clients[i].get_fd_client(),  "This client does not exist\n", 28, 0);
    else if (fd_rec != this->clients[i].get_fd_client())
    {
        this->clients[i].cmd[2].erase(0, 1);
        send(fd_rec,  this->clients[i].cmd[2].c_str(),  this->clients[i].cmd[2].size(), 0);
        send(fd_rec, "\n", 1, 0);
        this->clients[i].cmd.clear();
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

void Server::join_cmd(int i)
{
    int n_ch = this->found_channel(this->clients[i].cmd[1]);
    if (n_ch == -1)
    {
        Channel ch;
        ch.set_name(this->clients[i].cmd[1]);
        ch._Client.push_back(this->clients[i]);
        this->channels.push_back(ch);
        send(this->clients[i].get_fd_client(), "Creater channel \n", 17, 0);
    }
    else
    {
        this->channels[n_ch]._Client.push_back(this->clients[i]);
        send(this->clients[i].get_fd_client(), "ADD USER TO CHANNEL \n", 22, 0);
    }
}




void    Server::print(std::vector<std::string> &cmd, int fd)
{
    std::cout << "size " << cmd.size() << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
    {
        std::cout << "clien fd : " << this->clients[fd - 1].get_fd_client() << " with  :[" <<  cmd[i] << "]: ";
    }
    std::cout << std::endl;
}