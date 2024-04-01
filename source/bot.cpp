#include "../include/Header.hpp"

bool Server::bot(std::string &msg, int j,std::string &chan)
{
    std::string b_word = "fuck FUCK MOTHERFUCKER ASSHOLE"; // bad word not be on ch msg help me in this sring copilot
    std::vector<std::string> bad_words;
    std::vector<std::string> msg_word;
    bad_words = split_cmd(b_word);
    msg_word = split_cmd(msg);
    std::string reson = "using badwords on channel";
    for(size_t i = 0 ; i < bad_words.size();i++)
    {
        for(size_t l = 0 ; l < msg_word.size(); l++)
        {
            std::cout << msg_word[l] << ' ' << bad_words[i] <<'\n';
            if(msg_word[l] == bad_words[i])
            {
                std::cout << "L(WADA)"<<'\n';
                this->clients[j].cmd[0] = chan;
                this->clients[j].cmd[1] = chan;
                this->clients[j].cmd[2] = this->clients[j].get_nickname();
                this->clients[j].cmd[3] = reson;
                kick_command(j,1);
                return true;
            }
        }
    }
    return false;
}