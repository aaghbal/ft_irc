/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:51:32 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/14 10:34:36 by aaghbal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h> 
#include <vector>
#include <sys/socket.h>
#include <map>
#include "Error.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#define MAX_CLIENTS 10
#define INCCORRECT_PASS "ircserver 464 :Incorrect password\r\n"
#define PASS_REQUIRED "ircserver 461 :Password required\r\n"
#define PASS_ACCEPTED "ircserver 462 :Password accepted\r\n"
#define PASS_CHANGED "ircserver 463 :Password changed\r\n"
#define PASS_MISMATCH "ircserver 465 :Password mismatch\r\n"
#define PASS_TOO_LONG "ircserver 466 :Password too long\r\n"
#define PASS_TOO_SHORT "ircserver 467 :Password too short\r\n"
#define WELCOME_MSG ":ircserver 001 :Welcome to the Internet Relay Network\r\n"
#define NICKNAME_IN_USE "ircserver 433 :Nickname is already in use\r\n"
#define NICKNAME_CHANGED "ircserver 432 :Nickname changed\r\n"
#define NICKNAME_REQUIRED "ircserver 431 :No nickname given\r\n"
#define NICKNAME_TOO_LONG "ircserver 432 :Nickname too long\r\n"
#define NICKNAME_TOO_SHORT "ircserver 433 :Nickname too short\r\n"