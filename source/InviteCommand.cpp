/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:41:58 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/26 14:42:14 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

int Server::invite_check(std::string &nick, std::string &chan, int fd)
{
    std::cout << "nick : " << nick << " chan : " << chan << " fd " << fd << std::endl;
    if (nick.empty() || chan.empty())
        return (-1);
    int fd_rec = this->client_name[nick];
    std::cout << "hey " << fd_rec << std::endl;
    int ch = this->found_channel(chan);
    if(fd_rec == 0)
    {
        send(fd, "This client does not exist\r\n", 28, 0);
        return (-1);
    }
    if (ch == -1)
    {
        std::cout << ch << std::endl;
        send(fd, "This channel does not exist\r\n", 28, 0);
        return (-1);
    }
    if (check_client_channel(nick, ch, 0))
    {
        send(fd, "This client is allready in this Channel\r\n", 39, 0);
        return (-1);
    }
    return (fd_rec);
}