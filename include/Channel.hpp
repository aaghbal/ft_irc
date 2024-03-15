/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:26:15 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/15 16:03:27 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../include/Header.hpp"

class Channel
{
    private:
        std::string _Name;
        // time_t start_time;
    public:
        std::vector<Client> _Client;
        void    set_name(std::string const &name);
        // void Set_Name(std::string Name);
        // std::string & Get_Name();
        void Add_Client(Client &cli);
        // void rm_client(Client *cli);
        std::string &get_name_channel(void);
        // ~Channel();
};

