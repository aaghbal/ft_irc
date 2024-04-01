/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:51:32 by aaghbal           #+#    #+#             */
/*   Updated: 2024/03/31 15:54:24 by aaghbal          ###   ########.fr       */
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

#define MAX_CLIENTS 200

#define INCCORRECT_PASS " :Password incorrect\r\n"
#define PASS_REQUIRED "IRCsERVER 461 :Password required\r\n"
#define PASS_ACCEPTED "IRCsERVER 462 :Password accepted\r\n"
#define PASS_CHANGED "IRCsERVER 463 :Password changed\r\n"
#define PASS_MISMATCH "IRCsERVER 465 :Password mismatch\r\n"
#define PASS_TOO_LONG "IRCsERVER 466 :Password too long\r\n"
#define PASS_TOO_SHORT "IRCsERVER 467 :Password too short\r\n"
#define WELCOME_MSG " :Welcome to the IRCsERVER Network "
#define NICKNAME_IN_USE " :Nickname is already in use\r\n"
#define NICKNAME_CHANGED "IRCsERVER 432 :Nickname changed\r\n"
#define NICKNAME_REQUIRED "IRCsERVER 431 :No nickname given\r\n"
#define NICKNAME_TOO_LONG "IRCsERVER 432 :Nickname too long\r\n"
#define NICKNAME_TOO_SHORT "IRCsERVER 433 :Nickname too short\r\n"
