#include "../include/Header.hpp"

void Server::channel_topic(int i)
{
    std::string msg;
    int num_ch = found_channel(this->clients[i].cmd[1]);
    // std::cout << this->clients[i].cmd.size() << std::endl;
    // for (size_t j = 0; j < this->clients[i].cmd.size(); j++)
    //     std::cout << this->clients[i].cmd[j] << std::endl;
    // return ;
    if(this->clients[i].cmd.size() < 2)
    {
        msg = ":ircserver 461 TOPIC :Not enough parameters\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
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
    if(this->channels[num_ch].mode.find('t') != std::string::npos && !this->channels[num_ch].is_operator(this->clients[i].get_fd_client()) && this->clients[i].cmd.size() > 2)
    {
        msg = ":ircserver 442 " + this->channels[num_ch].get_name_channel() + " :You're not channel operator\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
        return ;
    }
    else
    {
        if(this->clients[i].cmd.size() < 3)
        {
            msg = ":ircserver 332 " + this->clients[i].get_nickname() + " " + this->channels[num_ch].get_name_channel() + " " +this->channels[num_ch].get_topic() + "\r\n";
            send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
            return ;
        }
        std::string topic;
        this->channels[num_ch].topic.clear();
        for (size_t j = 2; j < this->clients[i].cmd.size(); j++)
        {
            topic += this->clients[i].cmd[j] + " ";
        }
        this->channels[num_ch].topic = topic;
        msg = ":ircserver 332 " + this->clients[i].get_nickname() + " " + this->channels[num_ch].get_name_channel() + " " +this->channels[num_ch].get_topic() + "\r\n";
        send(this->clients[i].get_fd_client(), msg.c_str(), msg.size(), 0);
    }
}