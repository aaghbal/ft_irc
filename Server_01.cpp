#include "../include/Header.hpp"

void Server::invite_cmd(int i)
{
    if (this->clients[i].cmd.size() < 3 || (this->clients[i].cmd[0] != "INVITE"))
    {
        send(this->clients[i].get_fd_client(), "Syntax error : INVITE <nickname> <channel>\n", 42, 0);
        return ;
    }
    int fd = this->invite_check(this->clients[i].cmd[1], this->clients[i].cmd[2], this->clients[i].get_fd_client(), true) ;
    if(fd != -1)
    {
        send(fd, this->clients[i].get_nickname().c_str(), this->clients[i].get_nickname().size(), 0);
        send(fd, " have been invited you to join the channel\n", 43, 0);
    }
}

int Server::invite_check(std::string &nick, std::string &chan, int fd,bool mode)
{

    if (nick.empty() || chan.empty())
        return (-1);
    int fd_rec = this->client_name[nick];
    int ch = this->found_channel(chan);
    if(fd_rec == 0)
    {
        send(fd, "This client does not exist\n", 28, 0);
        return (-1);
    }
    if (ch == -1)
    {
        std::cout << ch << std::endl;
        send(fd, "This channel does not exist\n", 28, 0);
        return (-1);
    }
    if (mode && check_client_channel(nick, ch))
    {
        send(fd, "This client is allready in this Channel\n", 39, 0);
        return (-1);
    }
    for(size_t i = 0; i < this->channels[ch].Kiked_Client.size(); i++)
    {
        if (this->channels[ch].Kiked_Client[i] == nick)
        {
            send(fd, "This client has been kicked from this channel\n", 46, 0);
            return (-1);
        }
    }
    return (fd_rec);
}

bool Server::check_client_channel(std::string name,int ch_index)
{
    for (size_t i = 0; i < this->channels[ch_index]._Client.size(); i++)
    {
        if (this->channels[ch_index]._Client[i].get_nickname() == name)
            return true;
    }
    return false;
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

// void Server::join_cmd(int i)
// {
//     int n_ch = this->found_channel(this->clients[i].cmd[1]);
//     if (n_ch == -1)
//     {
//         Channel ch;
//         ch.set_name(this->clients[i].cmd[1]);
//         ch.set_owner(this->clients[i].get_nickname());
//         ch._Client.push_back(this->clients[i]);
//         this->channels.push_back(ch);
//         send(this->clients[i].get_fd_client(), "Creater channel \n", 17, 0);
//     }
//     else
//     {
//         for(size_t j = 0; j < this->channels[n_ch].Kiked_Client.size(); j++)
//         {
//             if (this->channels[n_ch].Kiked_Client[j] == this->clients[i].cmd[1])
//             {
//                 send(this->clients[i].get_fd_client(), "This client has been kicked from this channel\n", 46, 0);
//                 return;
//             }
//         }
//         this->channels[n_ch]._Client.push_back(this->clients[i]);
//         send(this->clients[i].get_fd_client(), "ADD USER TO CHANNEL \n", 22, 0);
//     }
// }

void Server::priv_msg_chan(int i, int j, bool flag)
{
    for (size_t k = 0; k < this->channels.size(); k++)
    {
        if (this->channels[k].get_name_channel() == this->clients[i].split_targ[j])
        {
            for (size_t c = 0; c < this->channels[k]._Client.size(); c++)
            {
                if ((this->channels[k]._Client[c].get_fd_client() != this->clients[i].get_fd_client()))
                {
                    if (flag)
                        send_all_arg(i, this->channels[k]._Client[c].get_fd_client());
                    else
                        send(this->channels[k]._Client[c].get_fd_client(),  this->clients[i].cmd[2].c_str(),  this->clients[i].cmd[2].size(), 0);
                }
            }
            return ;
        }
    }
    not_found_target_msg(i,j,0);
}

void Server::send_all_arg(int i, int fd_rec)
{
    for (size_t k = 2; k < this->clients[i].cmd.size() ; k++)
    {
        send(fd_rec,  this->clients[i].cmd[k].c_str(),  this->clients[i].cmd[k].size(), 0);
        send(fd_rec,  " ", 1, 0);
    }
}

std::vector<std::string> Server::split_cmd(std::string &cmd)
{
    std::vector<std::string>    MySplitData;
    std::string                 token;

    for(size_t i = 0; i < cmd.size() ; i++)
    {
        char c = cmd[i];
        if(c != ' ' && c != '\t')
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
    {
        this->clients[fd].split_targ.push_back(token);
        token.clear();
    }
}

// void Server::not_found_target_msg(int i, int j, int fla)
// {
//     send(this->clients[i].get_fd_client(),  "ircserv 401 ", 13, 0);
//     send(this->clients[i].get_fd_client(), this->clients[i].split_targ[j].c_str() , this->clients[i].split_targ[j].size(), 0);
//     if (fla)
//         send(this->clients[i].get_fd_client(),  " :No such nick\n", 15, 0);
//     else
//         send(this->clients[i].get_fd_client(),  " :No such channel\n", 18, 0);
        
// }

