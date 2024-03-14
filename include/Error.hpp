/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:15:24 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/08 11:09:31 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"

class Error {
  
    public:
      void ErrorArgument(void) const;   
      void ErrorGetAddrInof(void) const;
      void ErrorSocket(void) const;
      void ErrorBind(void) const;
      void ErrorListen(void) const;
      void ErrorAccept(void) const;
};
