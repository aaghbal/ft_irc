# IRC Server Protocol

This document provides an overview of a basic IRC server implementation using the C++ programming language.

## Overview

An IRC (Internet Relay Chat) server acts as a central network hub that facilitates communication between multiple clients. It manages connections, channels, and user commands, ensuring smooth and organized chat functionality.

## Components

The IRC server consists of several key components, each responsible for a specific aspect of the server's functionality:

### 1. Client
- **Information Storage**: The server stores essential information about each client, including their IP address. This is crucial for identifying and banning clients if necessary.
- **Connection and Welcome Message**: Clients are able to connect to the server and are greeted with a welcome message upon connection.
- **Private Chat**: The server enables two clients to engage in private conversations. This is achieved through the creation of a private 'channel' that is exclusive to the participating users.

### 2. Channel
- **Implementation**: Channels are implemented as ordered maps, tracking all active clients within each channel.
- **Broadcasting Messages**: When a message is sent to a channel, it is broadcast to all clients currently in that channel.
- **Banning Mechanism**: Users banned from a channel are unable to rejoin it using their nickname and IP address.

### 3. Commands
- **Functionality**: The server processes various commands that clients use to interact with it.
- **Command Objects**: Commands are implemented as objects that take a user's input and their parameters for execution.

## Conclusion

This IRC server aims to deliver a robust and efficient chat service, focusing on effective client management, seamless channel broadcasting, and responsive command processing. The use of C++ ensures a high-performance backend suitable for handling multiple client connections and real-time communication.

---
*Note: This README is a brief overview of the IRC server's architecture and functionality. For detailed documentation on code structure, installation, and usage, please refer to the respective sections of the project documentation.*
   ____________________________________
  |        IRC Server                  |               
  |____________________________________|                 
                    |                                 
                    | Accepts connections                   
                    V
   ____________________________________                              
  |   Connection Manager               |                       
  |____________________________________|                                
                    |                                       
                    | Manages channels                            
                    V                                       
   ____________________________________                                    
  |   Channel Manager                  |                                
  |____________________________________|                                      
                    |                                       
                    | Provides server functions                         
                    V                                       
   ____________________________________                              
  |   Server Functions                 |                                
  |____________________________________|                                   
                    |                                          
                    | Handles client interactions                       
                    V                                          
   ____________________________________                              
  |   Client Interface                 |                                      
  |____________________________________|                                


