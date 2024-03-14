/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:26:15 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/14 10:27:32 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Header.hpp"

class Channel
{
    private:
        std::string _Name;
        std::vector<Client *> _Client;
        time_t start_time;
    public:
        Channel(std::string Name , Client *cli);
        void Set_Name(std::string Name);
        std::string & Get_Name();
        void Add_Client(Client *cli);
        void rm_client(Client *cli);
        time_t get_Chennel_Time() const;
        ~Channel();
};

