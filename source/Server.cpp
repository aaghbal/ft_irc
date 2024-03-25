/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:47:14 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/23 16:59:16 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Header.hpp"

Server::Server()
{
    this->client_info.clear();
}

void Server::set_port(const char *port)
{
    this->port = port;
}

void Server::set_password(const std::string &password)
{
    this->password = password;
}

void Server::init_sockinfo(void)        
{
    this->sockinfo.sin_family= AF_INET;
    this->sockinfo.sin_port = htons(std::atoi(this->port));
    this->sockinfo.sin_addr.s_addr = INADDR_ANY;
}

void Server::create_socket(void)
{
    try
    {
        this->fd_s = socket(this->sockinfo.sin_family,  SOCK_STREAM, 0);
        if (this->fd_s == -1)
            throw Error();
        if (fcntl(this->fd_s, F_SETFL, O_NONBLOCK) == -1)
            throw Error();
    }
    catch(const Error& e)
    {
        e.ErrorSocket();
    }
}

void Server::bind_socket(void)
{
       try
    {
        int opt = 1;
        setsockopt(this->fd_s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if (bind(this->fd_s,(const sockaddr *)&this->sockinfo , sizeof(this->sockinfo)) == -1)
            throw Error();
    }
    catch(const Error& e)
    {
        e.ErrorBind();
    }  
}

void Server::listen_requ(void)
{ 
    try
    {
        if (listen(this->fd_s, 20) == -1)
            throw Error();
        this->polfd.push_back(init_pollfd(this->fd_s));
    }
    catch(const Error& e)
    {
        e.ErrorListen();
    }   
}

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

void Server::run_server()
{
    init_sockinfo();
    create_socket();
    bind_socket();
    listen_requ();
    accept_data();
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

// int    myRevc(std::string &str , int fd)
// {
//     int ret = 0;
//     str.clear();
//     while(str.find('\n') == std::string::npos)
//     {
//         char tmp[1000];
//         ret += recv(fd, tmp, 1000, 0);
//         if (ret == 0)
//             return -1;
//         tmp[ret] = '\0';
//         str += tmp;
//         memset(tmp, 0, 1000);
//     }
//     return ret;
// }

int    myRevc(std::string &str , int fd)
{
    char buff[1001];
    int ret = recv(fd, buff, 1000, 0);
    buff[ret - 1] = '\0';
    str = buff;
    return ret;
}


void Server::disconnect_client(int i)
{
    close(this->clients[i].get_fd_client());
    this->polfd.erase(polfd.begin() + i + 1);
    this->clients.erase(this->clients.begin() + i);
    // if (this->channels.size() != 0)
    //     this->channels.erase(this->channels.begin() + i - 1);
}

void Server::recive_data(int i)
{
    std::string message;
    this->nbyteread = myRevc(this->clients[i - 1].buff, this->clients[i - 1].get_fd_client());
    if (check_recv_message(i))
        return ;
    this->clients[i - 1].cmd = split_cmd(this->clients[i - 1].buff);
    if (!this->clients[i - 1].reg_end)
    {
        authenticate(i - 1); //hna canchecki wach kayn password wla nickname wla username
        return ;
    }
    switch (this->clients[i - 1].cmd[0][0])
    {
        case 'Q':
                disconnect_client(i - 1);
                break;
        case 'P':
                private_message(i - 1);
                break;
        case 'J':
                this->join_cmd(i - 1);
                break;
        case 'K':
                kick_command(i - 1);
                break;
        case 'M':
                Ch_modes(i - 1);
                break;
    }
        this->clients[i -1].cmd.clear();
    // }
}

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

bool Server::check_client_channel(std::string name,int ch_index, int flag)
{
    for (size_t i = 0; i < this->channels[ch_index]._Client.size(); i++)
    {
        if (this->channels[ch_index]._Client[i].get_nickname() == name)
        {
            if (flag)
                this->channels[ch_index]._Client.erase(channels[ch_index]._Client.begin() + i);
            return true;
        }
    }
    return false;
}

void Server::kick_command(int i)
{
    std::string msg;
    int num_ch = found_channel(this->clients[i].cmd[1]);
    if (num_ch == -1)
    {
        msg = ":ircserver 403 " + this->clients[i].cmd[1] + " : No such channel" + LF;
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    if (this->channels[num_ch].joined_in_channel(this->clients[i].get_fd_client()) == false)
    {
        msg = ":ircserver 441 " + this->clients[i].cmd[1] + " " + this->channels[num_ch].get_name_channel() + " :You're not on that channel" + LF;
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }//:irc.example.com 482 <channel> :You're not channel operator
    if (this->channels[num_ch].is_operator(this->clients[i].get_fd_client()) == false)
    {
        msg = ":ircserver 482 " + this->channels[num_ch].get_name_channel() + " :You're not channel operator" + LF;
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    erase_client_from_cha(i, num_ch);
    this->clients[i].split_targ.clear();
}

void Server::erase_client_from_cha(int i, int num_ch)
{
    split_target(this->clients[i].cmd[2], i);
    for (size_t j = 0; j < this->clients[i].split_targ.size() ; j++)
    {
        if (check_client_channel(this->clients[i].split_targ[j], num_ch, 1) == false)
        {
            send(this->clients[i].get_fd_client(), "ircserver 441 ", 14, 0);
            send(this->clients[i].get_fd_client(), this->clients[i].split_targ[j].c_str(), this->clients[i].split_targ[j].size(), 0);
            send(this->clients[i].get_fd_client(), " ", 1, 0);
            send(this->clients[i].get_fd_client(), this->clients[i].cmd[1].c_str(), this->clients[i].cmd[1].size(), 0);
            send(this->clients[i].get_fd_client(), " :They aren't on that channel\r\n", 30, 0); 
        }
    }
}
void Server::get_response_name(std::string &cmd, int i, int fd)
{
    std::string msg;
    msg += ":" + this->clients[i].get_nickname();
    msg += "!~";
    msg += this->clients[i].get_username();
    msg += '@';
    struct sockaddr_in t = this->client_info[fd];
    msg += inet_ntoa(t.sin_addr);
    if (this->clients[i].cmd[0] == "PRIVMSG")
        msg += " PRIVMSG ";
    else if (this->clients[i].cmd[0] == "JOIN")
        msg += " JOIN ";
    msg += cmd;
    msg += " ";
    msg += LF;
    send(fd, msg.c_str(), msg.size(), 0);
}

void    Server::priv_msg_user(int i, int j)
{
    int fd_rec = this->client_name[this->clients[i].split_targ[j]];
    if (this->clients[i].cmd[2].empty())
    {
        send(this->clients[i].get_fd_client(),  "No text to send\r\n", 16, 0);
        return ;
    }
    else if (fd_rec == 0)
        not_found_target_msg(i, j, 1);
    else if (fd_rec != this->clients[i].get_fd_client())
    {
        get_response_name(this->clients[i].split_targ[j], i, fd_rec);
        if (this->clients[i].cmd[2][0] == ':')
            send_all_arg(i, fd_rec);
        else
            send(fd_rec,  this->clients[i].cmd[2].c_str(),  this->clients[i].cmd[2].size(), 0);
        send(fd_rec, "\r\n", 1, 0);
    }   
}

pollfd Server::init_pollfd(int fd)
{
    struct  pollfd pfd;

    pfd.fd = fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    return pfd;
}


std::vector<std::string> Server::split_cmd(std::string &cmd)
{
    std::vector<std::string>    MySplitData;
    std::string                 token;

    for(size_t i = 0; i < cmd.size() ; i++)
    {
        char c = cmd[i];
        if(c != ' ' && c != '\t' && c != '\n' && c != '\r')
            token += c;
        else if (!token.empty())
        {
            MySplitData.push_back(token);
            token.clear();
        }
    }
    if (!token.empty())
        MySplitData.push_back(token);
    cmd.clear();
    return (MySplitData);
}

void Server::private_message(int i)
{
    split_target(this->clients[i].cmd[1], i);
    if (this->clients[i].split_targ.size() == 0)
    {
        send(this->clients[i].get_fd_client(),  "ircserv 411 :No recipient given PRIVMSG\r\n", 42, 0);
            return;
    }
    for(size_t j = 0 ; j < this->clients[i].split_targ.size() ; j++)
    {
        if (this->clients[i].split_targ[j][0] == '#')
            priv_msg_chan(i, j);
        else
            priv_msg_user(i, j);
    }
    this->clients[i].split_targ.clear();
}

int Server::found_channel(std::string const &chan)
{
    for(size_t i = 0; i < this->channels.size(); i++)
    {
        if (this->channels[i].get_name_channel() == chan)
            return (i);           
    }
    return (-1);
}

void Server::join_cmd(int i)
{
    std::string msg;
    if(this->clients[i].cmd[1].size() < 2)
    {
        msg = ":ircserver 461 " + this->clients[i].cmd[1] + " :Not enough parameters" + LF;
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    int n_ch = this->found_channel(this->clients[i].cmd[1]);
    if (n_ch == -1)
    {
        Channel ch;
        ch.set_name(this->clients[i].cmd[1]);
        ch._Client.push_back(this->clients[i]);
        ch.operat.push_back(this->clients[i].get_fd_client());
        this->channels.push_back(ch);
        //:irc.example.com 001 user123 :Welcome to the IRC server!
        msg = ":ircserver 001 " + this->clients[i].get_nickname() + WELCOME_MSG ;
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    }
    else if (this->channels[n_ch].mode.find('i') != std::string::npos)
    {
        send(this->clients[i].get_fd_client(), "ircserver 473 :Cannot join channel (+i)\r\n", 35, 0);
        return ;
    }
    else if (this->channels[n_ch].mode.find('k') != std::string::npos && this->clients[i].cmd.size() < 3)
    {
        send(this->clients[i].get_fd_client(), "ircserver 475 :Cannot join channel (+k)\r\n", 35, 0);
        return ;
    }
    else if (this->channels[n_ch].mode.find('l') != std::string::npos && this->channels[n_ch]._Client.size() == this->channels[n_ch].max_clients)
    {
        send(this->clients[i].get_fd_client(), "ircserver 471 :Cannot join channel (+l)\r\n", 35, 0);
        return ;
    }
    else if (this->channels[n_ch].joined_in_channel(this->clients[i].get_fd_client()))
    {
        send(this->clients[i].get_fd_client(), "ircserver 443 :Already in channel\r\n", 33, 0);
        return ;
    }
    else if (this->clients[i].cmd[1][0] == '#')
    {
        this->channels[n_ch]._Client.push_back(this->clients[i]);
        get_response_name(this->clients[i].cmd[1], i, this->clients[i].get_fd_client());
    }
    else if (this->clients[i].cmd[1][0] == '#')
    {
        for (size_t c = 0; c < this->channels[n_ch]._Client.size(); c++)
        {
            get_response_name(this->channels[n_ch].get_name_channel(), i, this->channels[n_ch]._Client[c].get_fd_client());  
            send(this->channels[n_ch]._Client[c].get_fd_client(), "\r\n", 1, 0);
        }
        this->channels[n_ch]._Client.push_back(this->clients[i]);
        get_response_name(this->channels[n_ch].get_name_channel(), i, this->clients[i].get_fd_client());
    }
}

void    Server::print(std::vector<std::string> &cmd, int fd)
{
    std::cout << "size " << cmd.size() << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
    {
        std::cout << "clien fd : " << this->clients[fd - 1].get_fd_client() << " with  :[" <<  cmd[i] << "]: ";
    }
    std::cout << std::endl;
}
void Server::split_target(std::string &cmd, int fd)
{
    std::string token = "";
    for(size_t i = 0; i < cmd.size(); i++)
    {
        char c = cmd[i];
        if(c != ',')
            token += c;
        else if (!token.empty())
        {
           
            this->clients[fd].split_targ.push_back(token);
            token.clear();
        }
    }
    if (!token.empty())
            this->clients[fd].split_targ.push_back(token);
}
void Server::not_found_target_msg(int i, int j, int fla)
{
    send(this->clients[i].get_fd_client(),  "ircserv 401 ", 13, 0);
    send(this->clients[i].get_fd_client(), this->clients[i].split_targ[j].c_str() , this->clients[i].split_targ[j].size(), 0);
    if (fla)
        send(this->clients[i].get_fd_client(),  " :No such nick\r\n", 15, 0);
    else
        send(this->clients[i].get_fd_client(),  " :No such channel\r\n", 18, 0);
        
}

void Server::send_all_arg(int i, int fd_rec)
{
    for (size_t k = 2; k < this->clients[i].cmd.size() ; k++)
    {
        send(fd_rec,  this->clients[i].cmd[k].c_str(),  this->clients[i].cmd[k].size(), 0);
        if ((k + 1) != this->clients[i].cmd.size())
            send(fd_rec,  " ", 1, 0);
    }
    send(fd_rec,  "\r\r\n", 2, 0);
}

void Server::priv_msg_chan(int i, int j)
{
    for (size_t k = 0; k < this->channels.size(); k++)
    {
        if (this->channels[k].get_name_channel() == this->clients[i].split_targ[j])
        {
            for (size_t c = 0; c < this->channels[k]._Client.size(); c++)
            {
                if ((this->channels[k]._Client[c].get_fd_client() != this->clients[i].get_fd_client()))
                {
                    get_response_name(this->clients[i].split_targ[j], i, this->channels[k]._Client[c].get_fd_client());
                    if (this->clients[i].cmd[2][0] == ':')
                        send_all_arg(i, this->channels[k]._Client[c].get_fd_client());
                    else
                        send(this->channels[k]._Client[c].get_fd_client(),  this->clients[i].cmd[2].c_str(),  this->clients[i].cmd[2].size(), 0);
                    send(this->channels[k]._Client[c].get_fd_client(),  "\r\n", 1, 0);
                }
            }
            return ;
        }
    }
    not_found_target_msg(i,j,0);
}

bool Server::check_client_name(std::string name)
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].get_nickname() == name)
            return true;
    }
    return false;
}

void Server::authenticate(int j)
{
    if (this->clients[j].cmd[0] == "PASS")
    {
        if (this->password == this->clients[j].cmd[1])
            this->clients[j].authenticate = true;
        else
        {
            std::string rec_mes = ":ircserver 464 " + this->clients[j].cmd[1] + INCCORRECT_PASS;
            send(this->clients[j].get_fd_client(), rec_mes.c_str() , rec_mes.size(), 0);
            return ;
        }
    }
    switch (this->clients[j].cmd.size())
    {
        case 2:
            if (this->clients[j].cmd[0] == "NICK" && this->clients[j].authenticate)
            {
                if (this->check_client_name(this->clients[j].cmd[1]))
                {
                    std::string rec_mes = ":ircserver 433 " + this->clients[j].cmd[1] + NICKNAME_IN_USE;
                    send(this->clients[j].get_fd_client(), rec_mes.c_str(), rec_mes.size(), 0);
                    return ;  
                }
                else
                {
                    this->clients[j].set_nickname(this->clients[j].cmd[1]);
                    this->client_name[this->clients[j].cmd[1]] = this->clients[j].get_fd_client();
                    this->clients[j].nick_succ = true;
                }
            }
        case 5:
            if(this->clients[j].cmd[0] == "USER" &&  this->clients[j].nick_succ) 
            {
                this->clients[j].set_username(this->clients[j].cmd[1]);
                this->clients[j].reg_end = true;
                std::string rec_mes = ":ircserver 001 " + this->clients[j].cmd[1] + WELCOME_MSG;
                send(this->clients[j].get_fd_client(), rec_mes.c_str(), rec_mes.size(), 0);
            }
        default:
           return ;
    }
}

//mode <channel|nickname> [[+|-]modechars [parameters]]

void Server::Ch_modes(int i)
{
    std::string allowed_modes = "itkl";
    int ch_index = found_channel(this->clients[i].cmd[1]);
    if (ch_index == -1)
    {
        send(this->clients[i].get_fd_client(), ":ircserver 403 :No such channel\r\n", 31, 0);
        return ;
    }
    for (size_t j = 2; j < this->clients[i].cmd.size(); j++)
    {
        if((this->clients[i].cmd[j][0] == '+' || this->clients[i].cmd[j][0] == '-' ) && (allowed_modes.find(this->clients[i].cmd[j][1]) != std::string::npos))
        {
            if (this->clients[i].cmd[j][1] == 'i' && this->clients[i].cmd[j + 1].empty())
            {
                this->channels[ch_index].mode += this->clients[i].cmd[j][0];
                return ;
            }   

            else if (this->clients[i].cmd[j][1] == 't' && this->clients[i].cmd[j + 1].empty())
            {
                this->channels[ch_index].mode += this->clients[i].cmd[j][0];
                return ;
            }   
            else if (this->clients[i].cmd[j][1] == 'k' && !this->clients[i].cmd[j + 1].empty() && this->clients[i].cmd[j + 1].size() <= 8)
            {
                std::cout << "cmd : " << this->clients[i].cmd[j + 1] << std::endl;
                this->channels[ch_index].password = this->clients[i].cmd[j + 1];
                return ;
            }
            else if (this->clients[i].cmd[j][1] == 'l' && !this->clients[i].cmd[j + 1].empty())
            {
                this->channels[ch_index].max_clients = std::atol(this->clients[i].cmd[j + 1].c_str());//check if the number is valid
                return ;
            }
            else if (this->clients[i].cmd[j][1] == 'o' && !this->clients[i].cmd[j + 1].empty())
                this->channels[ch_index].operat.push_back(this->clients[i].get_fd_client());
        }
        else
        {
            send(this->clients[i].get_fd_client(), ":ircserver 501 :Unknown MODE flag\r\n", 34, 0);
            return ;
        }
        std::cout << "cmd : " << this->clients[i].cmd[j] << std::endl;
    }
}