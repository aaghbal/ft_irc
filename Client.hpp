/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:25:16 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/11 16:44:44 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"

class Client {
  
    private :
        int             fd_client;
        std::string     nickname;
        std::string     username;
        
    public :
        void    set_nickname(std::string &niname);
        void    set_username(std::string &usname);
        void    set_fd_client(int fd);
};
