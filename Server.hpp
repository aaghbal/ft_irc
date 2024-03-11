/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:48:13 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/11 13:43:47 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"

class Server 
{
    private :
        int                                 fd_s;
        int                                 new_fd_s;
        const char                          *port;
        int                                 nbyteread;
        std::string                         password;
        struct sockaddr_in                  sockinfo;
        std::vector<struct pollfd>          polfd;
        std::vector<struct sockaddr_in>     client_info;
        socklen_t                           len;
        char                                buff[1000];
        

    public :
        void            init_sockinfo();
        void            create_socket();
        void            set_port(const char *port);
        void            bind_socket(void);
        void            listen_requ(void);
        void            accept_req(void);
        void            run_server();
        void            set_password(const std::string &password);
        bool            check_password(std::string &pass);
        struct  pollfd  init_pollfd(int fd);
        void            add_new_connection(void);
        void            recive_req(int i);
};