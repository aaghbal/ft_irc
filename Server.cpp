/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:47:14 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/11 15:03:31 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Header.hpp"
#include <sys/socket.h>
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
    set_port(this->port);
    init_sockinfo();
    create_socket();
    bind_socket();
    listen_requ();
    accept_req();
}

bool Server::check_password(std::string &pass)
{
    if(pass.back() == '\n')
            pass.pop_back();
    if (this->password == pass)
    {
        std::cout << "Welcome\n" << std::endl;
        send(this->new_fd_s,  "correct\n", 9, 0);
        return (true);
    }
    return (false);
}

void Server::add_new_connection(void)
{
    struct sockaddr_in clinfo;
    this->len = sizeof(clinfo);
    try
    {
        this->new_fd_s = accept(this->fd_s, (sockaddr *)&clinfo, &len);
        if (new_fd_s == -1)
            throw Error();
        this->polfd.push_back(init_pollfd(this->new_fd_s));
        this->client_info.push_back(clinfo);
        send(this->new_fd_s, "hello\n", 5, 0);
    }
    catch(const Error& e)
    {
        e.ErrorAccept();
    }

}

void Server::recive_req(int i)
{
    this->nbyteread = recv(this->polfd[i].fd, buff, 1000, 0);
    if (this->nbyteread <= 0)
    {
        if (this->nbyteread == 0)
            std::cout << "this client " << i << " closed" << std::endl;
        else
            std::cerr << "recv failed" << std::endl; 
        close(this->polfd[i].fd);
        this->polfd[i].fd = -1;
    }
    std::cout << buff << std::endl;
    bzero(buff, 1000);
}

pollfd Server::init_pollfd(int fd)
{
    struct  pollfd pfd;

    pfd.fd = fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    return pfd;
}
