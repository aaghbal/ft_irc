/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnAndRecvData.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:35:54 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/31 17:42:32 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

void Server:: accept_data(void)
{
    while (true)
    {
        int nm = poll(this->polfd.data(), this->polfd.size(), -1);
        if (nm == -1)
            break;
        if (nm == 0)
            continue;
        for(size_t i = 0; i < this->polfd.size(); i++)
        {
            if (this->polfd[i].revents & POLLIN)
            {
                if(this->polfd[i].fd == this->fd_s)
                    add_new_connection();
                else
                    recive_data(i);
            }
        }
    }
}

void Server::add_new_connection(void)
{
    struct sockaddr_in clinfo;
    this->len = sizeof(clinfo);
    Client   cl;
    try
    {
        int new_fd_s = accept(this->fd_s, (sockaddr *)&clinfo, &len);
        std::cout << "new client with fd : " << new_fd_s << std::endl;
        if (new_fd_s == -1)
            throw Error();
        if (this->clients.size() == this->max_clients)
        {
            std::string msg = ":IRCsERVER 421 * :Max clients reached\r\n";
            send(new_fd_s, msg.c_str(), msg.size(), 0);
            return ;
        }
        cl.set_fd_client(new_fd_s);
        this->clients.push_back(cl);
        this->polfd.push_back(init_pollfd(new_fd_s));
        this->client_info[new_fd_s] = clinfo;
    }
    catch(const Error& e)
    {
        e.ErrorAccept();
    }

}

int    myRevc(std::string &str , int fd)
{
    char buff[1001];
    int ret = recv(fd, buff, 1000, 0);
    if (ret > 0)
        buff[ret - 1] = '\0';
    str = buff;
    return ret;
}

void Server::recive_data(int i)
{
    std::string Ignored_cmd[] = {"NOTICE", "MOTD", "LUSERS", "VERSION", "STATS", "LINKS", "TIME", "CONNECT", "TRACE", "ADMIN", "INFO", "SERVLIST", "SQUERY", "WHO", "WHOIS", "WHOWAS", "KILL", "PING", "PONG", "ERROR", "AWAY", "REHASH", "DIE", "RESTART", "SUMMON", "USERS", "WALLOPS", "USERHOST", "ISON"};
    this->nbyteread = myRevc(this->clients[i - 1].buff, this->clients[i - 1].get_fd_client());
    if (check_recv_message(i))
        return ;
    this->clients[i - 1].cmd = split_cmd(this->clients[i - 1].buff);
    if (this->clients[i - 1].cmd.size() == 0)
        return ;
    to_upercase( this->clients[i - 1].cmd[0]);
    if (!this->clients[i - 1].reg_end)
    {
        authenticate(i - 1);
        return ;
    }
    for (size_t j = 0; j < sizeof(Ignored_cmd) / sizeof(Ignored_cmd[0]); j++)
    {
        if (this->clients[i - 1].cmd[0] == Ignored_cmd[j])
            return ;
    }
    switch (this->clients[i - 1].cmd[0][0])
    {
        case 'Q':
                if (this->clients[i - 1].cmd[0] == "QUIT")
                    disconnect_client(i - 1);
                break;
        case 'P':
                if (this->clients[i - 1].cmd[0] == "PRIVMSG")
                    private_message(i - 1);
                if (this->clients[i - 1].cmd[0] == "PASS")
                    Err_AlreadRegistred(i - 1);
                break;
        case 'J':
                if (this->clients[i - 1].cmd[0] == "JOIN")
                    join_cmd(i - 1);
                break;
        case 'K':
                if (this->clients[i - 1].cmd[0] == "KICK")
                    kick_command(i - 1,0);
                break;
        case 'M':
                if (this->clients[i - 1].cmd[0] == "MODE")
                    Ch_modes(i - 1);
                break;
        case 'I':
                if (this->clients[i - 1].cmd[0] == "INVITE")
                    InviteCommand(i - 1);
                break;
        case 'N':
                if (this->clients[i - 1].cmd[0] == "NICK")
                    change_nikname(i - 1);
                break;
        case 'U':
                if (this->clients[i - 1].cmd[0] == "USER")
                    Err_AlreadRegistred(i - 1);
                break;
        default:
        {
            std::string msg = ":IRCsERVER 421 " + this->clients[i - 1].get_nickname() + " " + this->clients[i - 1].cmd[0] + " :Unknown command\r\n";
            send(this->clients[i - 1].get_fd_client(), msg.c_str(), msg.size(), 0);
        }
    }
}

bool Server::check_recv_message(int i)
{
    if (this->nbyteread <= 0)
    {
        if (this->nbyteread == 0)
            std::cout << "this client " << i << " closed" << std::endl;
        else
            std::cerr << "recv failed" << std::endl; 
        close(this->polfd[i].fd);
        this->polfd.erase(polfd.begin() + i);
        this->clients.erase(this->clients.begin() + i - 1);
        // if (this->channels.size() != 0)
        //     this->channels.erase(this->channels.begin() + i - 1);
        return true;
    }
    return false;
}