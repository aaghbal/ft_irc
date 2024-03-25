/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:57:26 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/24 22:38:34 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server::joined_message(int fd, int i, int cha)
{
    std::string str;
    if (cha == -1)
    {
        get_response_name(clients[i].cmd[1], i, fd);
        send(fd, "\r\n", 2, 0);
        str = ":ircserver 353 " + clients[i].get_nickname() + " = " + clients[i].cmd[1] + " :@" + clients[i].get_nickname() + "\r\n";
        send(fd, str.c_str(), str.size(), 0);
        str.clear();
    }
    else
    {
        for(size_t j = 0; j < channels[cha]._Client.size(); j++)
        {
            get_response_name(clients[i].cmd[1], i, channels[cha]._Client[j].get_fd_client());
            send(channels[cha]._Client[j].get_fd_client(), "\r\n", 2, 0);
        }
        str = ":ircserver 353 " + clients[i].get_nickname() + " = " + clients[i].cmd[1] + " :";
        for(size_t k = 0; k < channels[cha]._Client.size(); k++)
        {
            if (channels[cha].is_operator(channels[cha]._Client[k].get_fd_client()))
                str += "@" + channels[cha]._Client[k].get_nickname();
            else
                str += channels[cha]._Client[k].get_nickname();
            if (k + 1 != channels[cha]._Client.size())
                str += " ";
            else
                str += "\r\n";
        }
        send(fd, str.c_str(), str.size(), 0);
        str.clear();
    }
    str = ":ircserver 366 " + clients[i].get_nickname() + " " + clients[i].cmd[1] + " :End of /NAMES list.\r\n";
    send(fd, str.c_str(), str.size(), 0);
    str.clear();
}