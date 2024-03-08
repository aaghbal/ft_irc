/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:48:13 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/08 11:20:46 by aaghbal          ###   ########.fr       */
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
        struct      addrinfo    sockinfo;
        struct      addrinfo    *adress;
        socklen_t               len;
        char                    buff[1000];
        

    public :
        void    init_addrinfo();
        void    create_socket();
        void    set_port(const char *port);
        void    bind_socket(void);
        void    listen_requ(void);
        void    accept_req(void);
        void    run_server();
};