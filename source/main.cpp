/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:04:03 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/28 12:27:51 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../include/Header.hpp"

int main(int ac, char **av)
{
    try
    {
        if (ac != 3)
            throw Error();
        parsing_port(atoi(av[2]));
        parsing(av[1], av[2]);
        Server s;
        s.set_port(atoi(av[2]));
        s.set_password(av[2]);
        s.run_server();
    }
    catch(const Error& e)
    {
        e.ErrorArgument();
    }
}

