/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:04:03 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/07 14:55:58 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"



int main(int ac, char **av)
{
    try
    {
        if (ac != 3)
            throw Error();
        char buffer[1024];
        struct addrinfo hint , *addr;
        memset(&hint, 0, sizeof(hint));
        hint.ai_family = AF_INET;
        hint.ai_socktype = SOCK_STREAM;
        hint.ai_flags = AI_PASSIVE;
        int status = getaddrinfo(NULL, av[1], &hint, &addr);
        if (status != 0)
            return(write(2,"error\n", 7));
        int fd_s = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        int s = bind(fd_s, addr->ai_addr, addr->ai_addrlen);
        if (s == -1)
            return(write(2,"error bind\n", 12));
        if (listen(fd_s, 10) == -1)
            return(write(2,"error listen\n", 14));
        socklen_t len = sizeof(addr->ai_addr);
        int new_fd = accept(fd_s, (struct sockaddr *)&addr->ai_addr, &len);
        if (new_fd == -1)
            return(write(2,"error accept\n", 14));
        while (true)
        {
            bzero(buffer, 1024);
            send(new_fd,  "password ", 10, 0);
            recv(new_fd, buffer, 1023, 0);
            if (strncmp("12345", buffer, 5) == 0)
            {
                std::cout << "Welcome\n" << std::endl;
                send(new_fd,  "correct\n", 9, 0);
            }
            else
                send(new_fd,  "incorrect check your password \n", 32, 0);
        }
        
    }
    catch(const Error& e)
    {
        e.ErrorArgument();
    }
}

