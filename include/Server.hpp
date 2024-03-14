/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:48:13 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/14 12:00:38 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"

class Client;

class Server 
{
    private :
        int                                 fd_s;
        const char                          *port;
        int                                 nbyteread;
        std::string                         password;
        std::string                         client_pass;
        struct sockaddr_in                  sockinfo;
        std::vector<struct pollfd>          polfd;
        std::vector<Client>                 clients;
        std::vector<struct sockaddr_in>     client_info;
        std::map<std::string, int>          client_name;
        socklen_t                           len;
        

    public :
        void                        init_sockinfo();
        void                        create_socket();
        void                        set_port(const char *port);
        void                        bind_socket(void);
        void                        listen_requ(void);
        void                        accept_data(void);
        void                        run_server();
        void                        set_password(const std::string &password);
        void                        check_password(int i);
        struct  pollfd              init_pollfd(int fd);
        void                        add_new_connection(void);
        void                        recive_data(int i);
        void                        init_client(int i);
        void                        recv_data(int i);
        std::vector<std::string>    split_cmd(std::string &cmd);
        void                        private_message(int i);
};