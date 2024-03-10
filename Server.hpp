/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:48:13 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/10 10:53:40 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"

class Server 
{
    private :
        int                     fd_s;
        int                     new_fd_s;
        const char              *port;
        std::string             password;
        struct sockaddr_in      sockinfo;
        struct sockaddr_in      client_info;
        socklen_t               len;
        char                    buff[1000];
        

    public :
        void    init_sockinfo();
        void    create_socket();
        void    set_port(const char *port);
        void    bind_socket(void);
        void    listen_requ(void);
        void    accept_req(void);
        void    run_server();
        void    set_password(const std::string &password);
        bool    check_password(std::string &pass);
};