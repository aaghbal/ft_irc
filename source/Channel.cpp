/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:26:23 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/14 14:50:09 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Header.hpp"


// Channel::Channel(std::string Name , Client cli)
// {
//     this->_Name = Name;
//     this->Add_Client(cli);
//     this->start_time = time(0);
// }


// Channel::~Channel()
// {
// }

// void Channel::Set_Name(std::string Name){
//     this->_Name = Name;
// };
// std::string & Channel::Get_Name(){
//     return this->_Name;
// };
// void Channel::Add_Client(Client cli){
//     this->_Client.push_back(cli);
// };
// void Channel::rm_client(Client cli){
//     for (size_t i = 0; i < this->_Client.size(); i++)
//     {
//         if (this->_Client[i]->get_nickname() == cli->get_nickname())
//             this->_Client.erase(this->_Client.begin() + i);
//     }
// };
// time_t Channel::get_Chennel_Time() const{
//     return this->start_time;
// };