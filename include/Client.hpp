/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:25:16 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/27 15:06:03 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client {
  
    private :
        int                                 fd_client;
        std::string                         nickname;
        std::string                         username;
        std::string                         password;
        
    public :
        Client();
        bool                                authenticate;
        bool                                reg_end;
        bool                                nick_succ;
        std::string                         buff;
        int                                 num_pass;

        std::vector<std::string>            cmd;
        std::vector<std::string>            split_targ;
        std::vector<std::string>            split_pass;
        void                                set_nickname(const std::string &niname);
        void                                set_username(const std::string &usname);
        void                                set_fd_client(int fd);
        void                                set_password(const std::string &pass);
        void                                set_num_attempts(void);

        int                                 get_num_attempts(void);
        int                                 get_fd_client(void);
        std::string                         &get_nickname(void);
        std::string                         &get_username(void);
        std::string                         &get_password(void);                     
};
#endif