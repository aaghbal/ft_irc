/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:26:15 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/29 13:35:46 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../include/Header.hpp"

class Channel
{
    private:
        std::string _Name;
    public:
        std::vector<std::string>    Kiked_Client;             
        std::string                 topic;
        std::vector<Client>         _Client;
        std::vector<int>            operat;
        std::vector<int>            invited;
        std::string                 mode;
        std::string                 password;
        size_t                      max_clients;
        int                         invite_check;
        void                        set_name(std::string const &name);
        void                        Add_Client(Client &cli);
        std::string                 &get_name_channel(void);
        bool                        is_operator(int fd);
        bool                        joined_in_channel(int fd);
        bool                        is_invited(int fd);
        void                        erase_operat(int fd);
        std::string                 &get_topic(void);
};

