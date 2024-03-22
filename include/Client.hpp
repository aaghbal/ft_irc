

#pragma once

#include "Header.hpp"

class Client {
  
    private :
        int                                 fd_client;
        std::string                         nickname;
        std::string                         username;
        std::string                         password;
        
    public :
        Client();
        bool                                authenticate;
        int                                 num_pass;
        bool                                info_client_fin;
        std::string                         buff;
        int                                 num_pass;

        std::vector<std::string>            cmd;
        std::vector<std::string>            split_targ;
        void                                set_nickname(const std::string &niname);
        void                                set_username(const std::string &usname);
        void                                set_fd_client(int fd);
        void                                set_password(const std::string &pass);
        void                                set_num_attempts(void);

        int                                 get_num_attempts(void);
        int                                 get_fd_client(void);
        std::string                         &get_nickname(void);
        std::string                         &get_username(void);
        std::string                         &get_password(void);                     
};
