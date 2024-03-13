/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:47:14 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/13 15:17:48 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"
#include "Server.hpp"

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

void Server::check_password(int i)
{
    if(this->clients[i].get_password().back() == '\n')
            this->clients[i].get_password().pop_back();
    if (this->password == this->clients[i].get_password())
    {
        send(this->clients[i].get_fd_client(),  "Welcome to the server\n", 22, 0);
        send(this->clients[i].get_fd_client(), "nickname ", 10, 0);
        this->clients[i].authenticate = true;
    }
    else
    {
        send(this->clients[i].get_fd_client(), "The password is incorrect, try again : ", 39, 0);
        this->clients[i].num_attempts++;
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
        send(new_fd_s, "connection successful\nEnter server password ", 45, 0);
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
    if (this->nbyteread <= 0)
    {
        if (this->nbyteread == 0)
            std::cout << "this client " << i << " closed" << std::endl;
        else
            std::cerr << "recv failed" << std::endl; 
        close(this->polfd[i].fd);
        this->polfd.erase(polfd.begin() + i);
        this->clients[i - 1].buff.clear();
    }
    std::cout << "clien fd : " << this->clients[i - 1].get_fd_client() << " with string : " <<  this->clients[i - 1].buff << std::endl;
    if (this->clients[i - 1].info_client_fin == false)
        init_client(i - 1);
    else
    {
        cmd = split_cmd(this->clients[i - 1].buff);
        int fd_rec = this->client_name[cmd[1]];
        send(fd_rec, cmd[2].c_str(), cmd[2].size(), 0);
        cmd.clear();
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
    if (this->clients[i].authenticate){
        if (this->clients[i].buff.empty()){
            if (this->clients[i].get_nickname().empty())
                send(this->clients[i].get_fd_client(), "please enter valid `nickname`: ", 31, 0);
            else if (this->clients[i].get_username().empty())
                send(this->clients[i].get_fd_client(), "please enter valid `username`:  ", 31, 0);
        }
        else if (this->clients[i].get_nickname().empty())
        {
            if (this->client_name.find(this->clients[i].buff) != this->client_name.end())
                send(this->clients[i].get_fd_client(), "this nickname had already taken Try another one: ", 50, 0);
            else
            {
                this->client_name[this->clients[i].buff] = this->clients[i].get_fd_client();
                this->clients[i].set_nickname(this->clients[i].buff);
                send(this->clients[i].get_fd_client(), "username ", 10, 0);
            }
        }
        else if (this->clients[i].get_username().empty())
        {
            this->clients[i].set_username(this->clients[i].buff);
            this->clients[i].info_client_fin = true;
            send(this->clients[i].get_fd_client(), "command ", 9, 0);
        }
    }
    if (this->clients[i].authenticate == false)
    {
        if (this->clients[i].num_attempts == 3)
        {
            close(this->clients[i].get_fd_client());
            this->polfd.erase(polfd.begin() + i + 1);
        }
        this->clients[i].set_password(this->clients[i].buff);
        check_password(i);
    }
}

std::vector<std::string> Server::split_cmd(std::string &cmd)
{
    std::vector<std::string>    MySplitData;
    std::string                 token;
    int                         nsp;
    int                         count = 0;

    while(cmd.find(' ') != std::string::npos)
    {
        if (count == 2)
            break;
        nsp = cmd.find(' ');
        token = cmd.substr(0, nsp);
        if (!token.empty() || !token.find(9))
        {
            MySplitData.push_back(token);
            count++;
        }
        cmd.erase(0, nsp + 1);
    }
    MySplitData.push_back(cmd);
    return (MySplitData);
}
