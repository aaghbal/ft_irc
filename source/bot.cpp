#include "../include/Header.hpp"

bool Server::bot(std::string &msg, int j,std::string &chan)
{
    to_lower(msg);
    std::string b_word = "fuck assman assmonkey assmunch assmuncher asspacker asspirate asspuppies assranger asswhore asswipe athletesfoot attack australian babe babies backdoor shit backdoorman backseat badfuck balllicker balls ballsack banging baptist barelylegal barf barface barfface bast bastard bazongas bazooms beaner beast beastality beastial beastiality beatoff beat-off beatyourmeat beaver bestial bestiality bi biatch bible bicurious bigass bigbastard bigbutt bigger bisexual bi-sexual bitch";
    std::vector<std::string> bad_words;
    std::vector<std::string> msg_word;
    bad_words = split_cmd(b_word);
    msg_word = split_cmd(msg);
    std::string reson = "bad_words";
    for(size_t l = 0 ; l < msg_word.size(); l++)
    {
        for(size_t i = 0 ; i < bad_words.size();i++)
        {
            if(msg_word[l][0] == ':')
                msg_word[l].erase(0,1);
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