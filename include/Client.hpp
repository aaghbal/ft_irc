/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:25:16 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/14 11:06:21 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"

class Client {
  
    private :
        int                                 fd_client;
        std::string                         nickname;
        std::string                         username;
        std::string                         password;
        
    public :
        bool                                authenticate;
        bool                                info_client_fin;
        std::string                         buff;
        int                                 num_attempts;
        std::vector<std::string>            cmd;
        void                                set_nickname(const std::string &niname);
        void                                set_username(const std::string &usname);
        void                                set_fd_client(int fd);
        void                                set_password(const std::string &pass);

        int                                 get_fd_client(void);
        std::string                         &get_nickname(void);
        std::string                         &get_username(void);
        std::string                         &get_password(void);
        Client();
};
