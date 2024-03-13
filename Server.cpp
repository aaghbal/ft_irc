/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:47:14 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/08 16:44:08 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Header.hpp"

void Server::set_port(const char *port)
{
    this->port = port;
}

void Server::set_password(const std::string &password)
{
    this->password = password;
}

void Server::init_addrinfo(void)
{
    this->sockinfo.ai_family = AF_INET;
    this->sockinfo.ai_socktype = SOCK_STREAM;
    this->sockinfo.ai_flags = AI_PASSIVE;
    try
    {
        if (getaddrinfo(NULL, this->port, &this->sockinfo, &this->adress) == -1)
            throw Error();
    }
    catch(const Error& e)
    {
        e.ErrorGetAddrInof();
    }
}

void Server::create_socket(void)
{
    try
    {
        this->fd_s = socket(this->adress->ai_family,  this->adress->ai_socktype, this->adress->ai_protocol);
        if (this->fd_s == -1)
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
        if (bind(this->fd_s, this->adress->ai_addr, this->adress->ai_addrlen) == -1)
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
    }
    catch(const Error& e)
    {
        e.ErrorListen();
    }   
}

void Server:: accept_req(void)
{
    this->len = sizeof(this->adress->ai_addr);
    try
    {
        this->new_fd_s = accept(this->fd_s, (struct sockaddr*)&this->adress->ai_addr, &len);
        if (this->new_fd_s == -1)
            throw Error();
    }
    catch(const Error& e)
    {
        e.ErrorAccept();
    }
     while (true)
    {
        bzero(buff, 1024);
        send(this->new_fd_s,  "password ", 10, 0);
        recv(this->new_fd_s, buff, 1023, 0);
        std::string buf = buff;
        if (check_password(buf))
            break;
        else
            send(this->new_fd_s,  "incorrect check your password \n", 32, 0);
    }
}

void Server::run_server()
{
    set_port(this->port);
    init_addrinfo();
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