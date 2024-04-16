#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h> 
#include <vector> 
class bot {
private:
    std::string server;
    int port;

public:
    struct pollfd fds[1];
    std::string password;
    std::string data;
    bot(std::string server, int port,std::string password,std::string nickname,std::string channel_name);
    void connect();
    bool Joined_ch;
    bool auth;
    std::string channel_name;
    std::string nickname;
    void send_data(const std::string& data);
    void receive_data();
    ~bot();
    void join_channel();
    bool check_mode();
    void kick_client();
    void auth_login(std::string password);
};
