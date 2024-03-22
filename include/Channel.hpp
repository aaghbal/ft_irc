/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:26:15 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/19 13:23:56 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../include/Header.hpp"

class Channel
{
    private:
        std::string _Name;
        
    public:
        std::vector<std::string> Kiked_Client;
        std::vector<Client> _Client;
        std::vector<int>    operat;
        void                set_name(std::string const &name);
        void                Add_Client(Client &cli);
        std::string         &get_name_channel(void);
        bool                is_operator(int fd);
        bool                joined_in_channel(int fd);
        
        
};

