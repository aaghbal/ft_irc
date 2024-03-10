/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:47:14 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/10 11:00:17 by aaghbal          ###   ########.fr       */
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
    }
    catch(const Error& e)
    {
        e.ErrorListen();
    }   
}

void Server:: accept_req(void)
{
    this->len = sizeof(this->client_info);
    try
    {
        this->new_fd_s = accept(this->fd_s, (struct sockaddr*)&client_info, &len);
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
        if (check_password(buf));
        else
            send(this->new_fd_s,  "incorrect check your password \n", 32, 0);
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