/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:47:14 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/07 11:54:55 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Header.hpp"

void Server::set_port(const char *port)
{
    this->port = port;
}

void Server::create_socket(void)
{
    this->sockinfo.ai_family = AF_INET;
    this->sockinfo.ai_socktype = SOCK_STREAM;
    this->sockinfo.ai_flags = 0;
    getaddrinfo(NULL, this->port, &this->sockinfo, &this->adress);
    this->fd_s = socket(adress->ai_family, adress->ai_socktype, 0);
    std::cout << this->fd_s << std::endl;
    std::cout << (const sockaddr *)&this->adress->ai_addr->sa_data << std::endl;
}

void Server::bind_socket(void)
{
    bind(this->fd_s,(const sockaddr *)&this->adress->ai_addr, sizeof(this->adress->ai_addr));   
}

void Server::listen_requ(void)
{
    listen(this->fd_s, 20);   
}

void Server:: accept_req(void)
{
    this->new_fd_s = accept(this->fd_s, (struct sockaddr*)&this->adress->ai_addr, (socklen_t *)sizeof(adress->ai_addr));
    while(1)
    {
        // puts("here");
        send(this->new_fd_s, "password", 8, 0);
        recv(this->new_fd_s, this->buff, 2047, 0);
        if (strncmp(this->buff, "1234", 4) == 0)
            send(this->new_fd_s, "done", 4, 0);
        else
            send(this->new_fd_s, "try again\n", 10, 0);
        bzero(this->buff, 2048);
    }
}