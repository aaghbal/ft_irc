#include "bot.hpp"

const int BUFFER_SIZE = 2048;

bot::bot(std::string server, int port,std::string password,std::string nickname,std::string channel_name){
    this->server = server;
    this->port = port;
    this->password = password;
    this->nickname = nickname;
    this->channel_name = channel_name;
};

bot::~bot() {
    close(fds[0].fd);
}

void bot::receive_data() {
    char buffer[BUFFER_SIZE];
    data.clear();
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(fds[0].fd, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        if (errno == EAGAIN) 
            return ;
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }
    if (bytes_received == 0) {
        perror("connection closed");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    data = buffer;
}


void bot::send_data(const std::string& data) {
    if (send(fds[0].fd, data.c_str(), data.length(), 0) < 0) {
        perror("Error sending data");
        exit(EXIT_FAILURE);
    }
}

void bot::join_channel() {
    send_data("JOIN " + channel_name + " \r\n");
    sleep(1);
    receive_data();
    if(data.find("353") != std::string::npos || data.find("443") != std::string::npos){
        std::cout << "Joined channel" << std::endl;
        this->Joined_ch = true;
        return;
    }
    std::cout << data << std::endl;
    exit(EXIT_FAILURE);
}

void bot::auth_login(std::string password)
{
    send_data("PASS " + password + "\r\n");
    sleep(1);
    send_data("NICK "+ nickname +" \r\n"); 
    sleep(1);
    send_data("USER "+ nickname +" 0 * :"+ nickname +"\r\n");
    sleep(1);
    receive_data();
    for (size_t i = 0; i < 10; i++)
    {
        if(data.find("001") != std::string::npos)
        {
            auth = true;
            return;
        }
        sleep(1);
    }
    std::cout << "Authentification: Failed" << auth << std::endl;
    exit(EXIT_FAILURE);
}

bool bot::check_mode()
{
    send_data("MODE " + channel_name + " +o Bot\r\n"); 
    sleep(1);
    receive_data();
    if(data.find("482") != std::string::npos)
        return true;
    return false;
}

void bot::connect() {
    struct sockaddr_in myaddr;
    int sockfd; // Corrected variable name

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);
    inet_aton(server.c_str(), &myaddr.sin_addr); 

    sockfd = socket(myaddr.sin_family , SOCK_STREAM, 0); 
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("Error setting socket to non-blocking mode");
        exit(EXIT_FAILURE);
    }
    int result = ::connect(sockfd, (struct sockaddr*)&myaddr, sizeof(myaddr));
    if (result < 0 && errno != EINPROGRESS) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }
    fds[0].fd = sockfd;
    fds[0].events = POLLOUT;
    result = poll(fds, 1, 5000);
    if (result < 0) {
        perror("Error in poll");
        exit(EXIT_FAILURE);
    } else if (result == 0) {
        std::cerr << "Timeout waiting for connection" << std::endl;
        exit(EXIT_FAILURE);
    } else {
        if (fds[0].revents & POLLOUT) {
            std::cout << "Connected to server" << std::endl;
        } else 
        {
            std::cerr << "Error connecting to server" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

int count_char(std::string &port)
{
    int c = 0;
    for (size_t i = 0; i < port.size(); i++)
    {
        if (port[i] == '+')
          c++;  
    }
    if (c > 1)
        throw std::runtime_error("Error: Port syntax error");
    return (c);
}

void parsing(std::string password, std::string port)
{
    if (port.find_first_not_of("0123456789+") != std::string::npos)
        throw std::runtime_error("Error: Port contains invalid characters");
    if (count_char(port) == 1 && port[0] != '+')
        throw std::runtime_error("Error: Port syntax error");
    if (password.size() > 9 || password.size() < 2)
        throw std::runtime_error("Error: Password size must be between 2 and 9 characters");
    if (port.size() == 0)
        throw std::runtime_error("Error: Port size is invalid");
}

void    parsing_port(int port)
{
    if (port < 1 || port > 65535)
        throw std::runtime_error("Error: Port out of range");
}

int main(int ac, char **av) {
    if(ac != 5)
    {
        std::cerr << "Usage: ./bot <Password> <Port> <nickname> <channel>" << std::endl;
        exit(EXIT_FAILURE);
    }
    try
    {
        if(std::string(av[4]).size() == 0 || std::string(av[3]).size() == 0 || std::string(av[4]).find("#") == std::string::npos)
            throw std::runtime_error("Error: Parameter is empty or channel need # prefix");
        parsing_port(atoi(av[2]));
        parsing(av[1], av[2]);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string password = av[1];
    int port = std::stoi(av[2]);
    std::string nickname = av[3];
    std::string channel_name =av[4];
    bot bot("0.0.0.0", port , password, nickname ,channel_name);
    bot.connect();
    bot.Joined_ch = false;
    bot.auth = false;
    bot.auth_login(password);
    sleep(1);
    if(bot.check_mode() == false)
    {
        std::cerr << "Channel don't exsist" << std::endl;
        exit(EXIT_FAILURE);
    }
    bot.join_channel();
    while (true)
    {
        int pool = poll(bot.fds, 1, 100);
        if (pool < 0) {
            perror("Error in poll");
            exit(EXIT_FAILURE);
        } else if (pool == 0) {
            std::cerr << "Timeout waiting for connection" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (bot.fds[0].revents & POLLOUT) 
        {
            if(bot.Joined_ch == false)
                exit(EXIT_FAILURE);
            std::string msg = "PRIVMSG " + bot.channel_name + " :Welcome To the Channel From BOT \r\n";
            bot.send_data(msg);
            sleep(20);
            bot.receive_data();
        }
    }
    return 0;
}
