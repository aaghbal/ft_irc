#include "../include/Header.hpp"

void Server::channel_topic(int i)
{
    std::string msg;
    int num_ch = found_channel(this->clients[i].cmd[1]);
    if (num_ch == -1)
    {
        msg = ":ircserver 403 TOPIC :No such channel\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    if(check_client_channel(this->clients[i].get_nickname(), num_ch, 0, 0) == -1)
    {
        msg = ":ircserver 442 " + this->clients[i].cmd[1] + " :You're not on that channel\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    msg = ":ircserver 332 " + this->clients[i].get_nickname() + " " + this->channels[num_ch].get_name_channel() + " :" + this->channels[num_ch].get_topic() + "\r\n";
    send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
}