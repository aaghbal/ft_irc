
ft_irc

This project is an implementation of an IRC server written in C++.

Description

Internet Relay Chat (IRC) is a protocol for real-time communication over a network. It allows users to join channels and chat with others in a group setting, or send private messages directly to other users.

This project implements a basic IRC server that can handle:

Multiple concurrent clients

User registration and authentication

Joining and leaving channel

Sending messages to channels and private message

(Optional) Additional features like file transfer and channel moderation
Requirements

C++ compiler (C++98 standard or later)
Make build system
Building

Clone the repository:
git clone https://github.com/aaghbal/ft_irc
Compile the project:
make

Running

Start the server:

./server port [password]

Replace port with the port number you want the server to listen on (default: 6667)

password is an optional argument to set a password for connecting to the server

Connect to the server using an IRC client software like irssi or mIRC.

Supported Commands

/nick: Change your nickname

/join: Join a channel

/part: Leave a channel

/users: Get a list of users on the server

/names: Get a list of users in a channel

/msg <nickname>: Send a private message to a user

/quit: Disconnect from the server
