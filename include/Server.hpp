/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:48:13 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/20 16:56:37 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../include/Header.hpp"

class Client;
class Channel;

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
        std::map<int, struct sockaddr_in>   client_info;
        std::map<std::string, int>          client_name;
        std::vector<Channel>                channels;
        socklen_t                           len;
        

    public :
        Server();
        void                        init_sockinfo();
        void                        create_socket();
        void                        set_port(const char *port);
        void                        bind_socket(void);
        void                        listen_requ(void);
        void                        accept_data(void);
        void                        run_server();
        void                        set_password(const std::string &password);
        bool                        check_recv_message(int i);
        void                        check_password(int i);
        struct  pollfd              init_pollfd(int fd);
        void                        add_new_connection(void);
        void                        recive_data(int i);
        void                        init_client(int i);
        void                        recv_data(int i);

        std::vector<std::string>    split_cmd(std::string &cmd);
        void                        private_message(int i);
        int                         found_channel(std::string const &chan);
        void                        join_cmd(int i);
        void                        print(std::vector<std::string> &cmd, int fd);
        void                        split_target(std::string &cmd, int i);
        void                        not_found_target_msg(int i, int j, int fla);
        void                        send_all_arg(int i, int fd_rec);
        void                        priv_msg_chan(int i, int j);
        int                         invite_check(std::string &nick, std::string &chan, int fd);
        bool                        check_client_channel(std::string name,int fd, int flag);
        void                        kick_command(int i);
        void                        erase_client_from_cha(int i, int num_ch);
        void                        get_response_name(std::string &cmd, int i, int fd);
        void                        priv_msg_user(int i, int j);                    
};
