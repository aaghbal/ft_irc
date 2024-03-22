

#include "../include/Client.hpp"

Client::Client()
{
    std::cout << "herrrreee" << std::endl;
    this->authenticate = false;
    this->nickname = "";
    this->username = "";
    this->num_pass = 0;
    this->info_client_fin = false;
    this->split_targ.clear();
}
void Client::set_nickname(const std::string &niname)
{
    this->nickname = niname;
}

void Client::set_username(const std::string &usname)
{
    this->username = usname;
}

void Client::set_fd_client(int fd)
{
    this->fd_client = fd;       
}

void Client::set_password(const std::string & pass)
{
    this->password = pass;
}

int Client::get_fd_client(void)
{
    return(this->fd_client);
}


std::string &Client::get_nickname(void)
{
    return (this->nickname);
}

std::string &Client::get_username(void)
{
    return (this->username);
}

std::string & Client::get_password(void)
{
    return (this->password);
}
