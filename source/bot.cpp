#include "../include/Header.hpp"

bool Server::bot(std::string &msg, int j,std::string &chan)
{
    std::string b_word = "fuck FUCK MOTHERFUCKER ASSHOLE"; // bad word not be on ch msg help me in this sring copilot
    std::vector<std::string> bad_words;
    std::vector<std::string> msg_word;
    bad_words = split_cmd(b_word);
    msg_word = split_cmd(msg);
    std::string reson = "bad_words";
    for(size_t i = 0 ; i < bad_words.size();i++)
    {
        for(size_t l = 0 ; l < msg_word.size(); l++)
        {
            if(msg_word[l] == bad_words[i])
            {
                this->clients[j].cmd.clear();
                this->clients[j].cmd.push_back("KICK");
                this->clients[j].cmd.push_back(chan);
                this->clients[j].cmd.push_back(this->clients[j].get_nickname());
                this->clients[j].cmd.push_back(reson);
                kick_command(j,1);
                bad_words.clear();
                msg_word.clear();
                return true;
            }
        }
    }
    return false;
}