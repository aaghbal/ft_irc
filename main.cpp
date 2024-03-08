/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:04:03 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/08 16:42:38 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"



int main(int ac, char **av)
{
    try
    {
        if (ac != 3)
            throw Error();
        Server s;
        
        s.set_port(av[1]);
        s.set_password(av[2]);
        s.run_server();
        std::cout << "is finished" << std::endl;
    }
    catch(const Error& e)
    {
        e.ErrorArgument();
    }
}

