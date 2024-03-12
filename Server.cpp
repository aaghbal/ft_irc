/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:47:14 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/12 15:42:40 by aaghbal          ###   ########.fr       */
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

void Server:: accept_req(void)
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
                    recive_req(i);
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
    accept_req();
}

void Server::check_password(int i)
{
    if(this->clients[i].get_password().back() == '\n')
            this->clients[i].get_password().pop_back();
    if (this->password == this->clients[i].get_password())
    {
        send(this->clients[i].get_fd_client(),  "Welcome to the server ", 22, 0);
        this->clients[i].authenticate = true;
    }
    else
        send(this->clients[i].get_fd_client(), "The password is incorrect, try again : ", 39, 0);
}

void Server::add_new_connection(void)
{
    struct sockaddr_in clinfo;
    this->len = sizeof(clinfo);
    Client   cl;
    try
    {
        this->new_fd_s = accept(this->fd_s, (sockaddr *)&clinfo, &len);
        if (new_fd_s == -1)
            throw Error();
        send(this->new_fd_s, "connection successful\nEnter server password ", 45, 0);
        cl.set_fd_client(this->new_fd_s);
        this->clients.push_back(cl);
        this->polfd.push_back(init_pollfd(this->new_fd_s));
        this->client_info.push_back(clinfo);
    }
    catch(const Error& e)
    {
        e.ErrorAccept();
    }

}

void Server::recive_req(int i)
{
    bzero(this->clients[i - 1].buff, 1000);
    this->nbyteread = recv(this->clients[i - 1].get_fd_client(), this->clients[i - 1].buff, 1000, 0);
    if (this->nbyteread <= 0)
    {
        if (this->nbyteread == 0)
            std::cout << "this client " << i << " closed" << std::endl;
        else
            std::cerr << "recv failed" << std::endl; 
        close(this->polfd[i].fd);
        this->polfd.erase(polfd.begin() + i);
    }
    this->clients[i - 1].buff[nbyteread - 1] = '\0';
    init_client(i - 1);
    
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
    if (this->clients[i].authenticate == false)
    {
        this->clients[i].set_password(this->clients[i].buff);
        check_password(i);
    }
    else if (this->clients[i].get_nickname().empty())
    {
        send(this->clients[i].get_fd_client(), "nickname ", 10, 0);
        this->clients[i].set_nickname(this->clients[i].buff);
    }
    else if (this->clients[i].get_username().empty())
    {
        send(this->clients[i].get_fd_client(), "username ", 10, 0);
        this->clients[i].set_username(this->clients[i].buff);
    }
}
