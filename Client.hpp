/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:25:16 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/12 13:16:07 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"

class Client {
  
    private :
        int             fd_client;
        std::string     nickname;
        std::string     username;
        std::string     password;
        
    public :
        char    buff[1000];
        void            set_nickname(const std::string &niname);
        void            set_username(const std::string &usname);
        void            set_fd_client(int fd);
        int             get_fd_client(void);
        void            set_password(const std::string &pass);
        std::string     &get_password(void);
        // void    init_client(void);
};
