/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:48:13 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/31 17:20:40 by aaghbal          ###   ########.fr       */
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
        int                                 port;
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
        void                        set_port(int port);
        void                        bind_socket(void);
        void                        listen_requ(void);
        void                        accept_data(void);
        void                        run_server();
        void                        set_password(const std::string &password);
        bool                        check_recv_message(int i);
        void                        check_password(int j);
        void                        check_nickname(int j);
        void                        check_username(int j);
        void                        change_nikname(int i);
        struct  pollfd              init_pollfd(int fd);
        void                        add_new_connection(void);
        void                        recive_data(int i);
        void                        init_client(int i);
        void                        recv_data(int i);
        void                        Command_Responses(int i);
        bool                        parsing_nickname(int i);
        bool                        unk_com;

        std::vector<std::string>    split_cmd(std::string &cmd);
        void                        private_message(int i);
        int                         found_channel(std::string const &chan);
        void                        join_cmd(int i);
        void                        split_target(std::string &cmd, int i, int flag);
        void                        not_found_target_msg(int i, int j, int fla);
        void                        not_found_target_chan(int i, int k);
        void                        all_arg(std::string &msg, int i);
        void                        priv_msg_chan(int i, int j);
        int                         invite_check(std::string &nick, std::string &chan, int fd,int i);
        int                         check_client_channel(std::string name,int ch_index, int flag,int kicker);
        void                        kick_command(int i);
        void                        erase_client_from_cha(int i, int num_ch);
        void                        get_response_join(std::string &cmd, int i, int fd);
        void                        get_response_nick(std::string &cmd, int i, int fd);
        void                        get_response_privmsg(std::string &msg, std::string &cmd, int i, int fd);
        void                        GetUserChannel(std::string &msg, int ch);
        void                        priv_msg_user(int i, int j);    
        void                        authenticate(int j);                
        bool                        check_client_name(std::string name);
        void                        disconnect_client(int i);
        void                        create_new_chan(int i, int k);
        bool                        check_mode_chan(int n_ch, int i);
        void                        join_channel(int n_ch, int i, int k);
        void                        joined_message(int fd, int i, int cha, int k);
        void                        Ch_modes(int i);
        void                        to_upercase(std::string &cmd);
        void                        password_channel(std::string &pass);
        void                        InviteCommand(int i);
        void                        channel_topic(int i);
        void                        CannotendToChan(int i, int j);
        void                        Err_NeedMoreParam(int i);
        void                        ErrBadChannelKey(int i, int k);
        void                        Err_AlreadRegistred(int i);
        void                        No_NicknameGiven(int i);
        void                        Err_NotRegistered(int i);
        void                        ErrOneusNickname(int i);

};
