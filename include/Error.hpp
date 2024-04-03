/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:15:24 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/31 13:56:46 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
#define ERROR_HPP

#include "Server.hpp"


void parsing(std::string password, std::string port);
void parsing_port(int port);

class Error {
  
    public:
      void ErrorArgument(void) const;   
      void ErrorGetAddrInof(void) const;
      void ErrorSocket(void) const;
      void ErrorSetsockopt(void) const;
      void ErrorBind(void) const;
      void ErrorListen(void) const;
      void ErrorAccept(void) const;
      void ErrorSizePass(void) const;
      void ErrorPort(void) const;
      void ErrorSizePort(void) const;
      void ErrorRangePort(void) const;
      void ErrorPortSyn(void) const;
      
};
#endif