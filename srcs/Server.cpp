/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/30 12:09:41 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

Server::Server(char *av1, char *av2) : _port(atoi(av1)), _password(av2), _fdsCounter(0)
{}

Server::~Server()
{}

void    Server::setSockFd()
{
    _serverFd = socket(PF_INET, SOCK_STREAM, 0); // get the fd for the server
    if (_serverFd == -1)
    {
        perror("Error");
        throw std::runtime_error("Socket Error");
    }
    std::cout << GREEN << "Socket created successfully" << RESET << std::endl;
}

void    Server::setAddr()
{
    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
    {
        close(_serverFd);
        perror("Error");
        throw std::runtime_error("Setting socket options Error");
    }
    memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;  // ipv4
    _servAddr.sin_addr.s_addr = INADDR_ANY; // accept connections on any of the available network interfaces on the machine
    _servAddr.sin_port = htons(_port); // convert port from host byte order to network byte order
    std::cout << GREEN << "Server configuration done successfully, Ready to bind" << RESET << std::endl;
}

void    Server::bindServ()
{
    if (bind(_serverFd, (struct sockaddr *) &_servAddr, sizeof(_servAddr)) == -1) // bind to the port using the addr struct
    {
        close(_serverFd);
        perror("Error");
        throw std::runtime_error("Binding Error");
    }
    std::cout << GREEN << "Server binded successfully" << RESET << std::endl;
}

void    Server::listenServ()
{
    if (listen(_serverFd, MAX_CONNECTIONS) == -1)
    {
        close(_serverFd);
        perror("Error");
        throw std::runtime_error("Listening Error");
    }
    std::cout << GREEN << "Server is listening successfully" << RESET << std::endl;
    _polls[0].fd = _serverFd;
    _polls[0].events = POLLIN;
    _polls[0].revents = 0;
    _fdsCounter++;
}

void    Server::acceptAndAddConnections()
{
    socklen_t _servAddrLen = sizeof(_servAddr);
    if ((_newFd = accept(_serverFd, (struct sockaddr *) &_servAddr, &_servAddrLen)) == -1)
    {
        close(_serverFd);
        perror("Error");
        throw std::runtime_error("Accepting a new connection error");
    }
    _polls[_fdsCounter].fd = _newFd;
    _polls[_fdsCounter].events = POLLIN;
    _fdsCounter++;
    std::cout << "Server accepted a connection" << std::endl;
    send(_newFd, "WELCOME TO THE SERVER\r\n", 24, 0);
}

void    Server::setAndRunServ()
{
    setSockFd();
    setAddr();
    bindServ();
    listenServ();
    std::cout << GREEN << "Server is running" << RESET << std::endl;
    while (shouldRun)   // The main server loop
    {
       //printFdStruct();
        int numEvents = poll(_polls, _fdsCounter, 3000);
        std::cout << "NUMBER OF FD'S: " << _fdsCounter << std::endl;
        std::cout << "NUMBER OF EVENTS: " << numEvents << std::endl;
        if (numEvents < 0)
        {
            close(_serverFd);
            perror("Error");
            throw std::runtime_error("Poll Error");
        }
        else if (numEvents > 0)
        {
            std::cout << "Poll is monitoring" << std::endl;
            // Check existing connections looking for data to read //
            for (int i = 0; i < _fdsCounter; i++)
            {
                // Check if someone is ready to read // 
                if (_polls[i].revents & POLLIN)
                {
                    // if server we handle the new connection //
                    if (_polls[i].fd == _serverFd)
                    {
                        std::cout << GREEN << "Server ready to accept" << RESET << std::endl;
                        acceptAndAddConnections();
                    }
                    else        // it is just a regular client
                    {
                        std::cout << " I AM A REGULAR CLIENT " <<  std::endl;
                        int bytes = recv(_polls[i].fd, _buffer, sizeof(_buffer), 0);
                        std::cout << "MESSAGE FROM CLIENT: " << _buffer << std::endl;
                        int senderFd = _polls[i].fd;
                        
                        // Connection closed or Error
                        if (bytes <= 0)
                        {
                            if (!bytes)
                                std::cout << RED << "Connections closed " << senderFd << " hung up" << RESET << std::endl;   
                            else
                                perror("Error");
                            close(_polls[i].fd);
                        }
                        else // We got some good data from a client
                        {
                            for (int j = 0; j < _fdsCounter; j++) // send everyone
                            {
                                int destFd = _polls[j].fd;
                                // Except the listener and ourselves
                                if (destFd != senderFd) 
                                {
                                    if (send(destFd, _buffer, bytes, 0) == -1)
                                        perror("send");
                                    std::cout << "HERE IS THE SENDER " << _buffer << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void    Server::printFdStruct()
{
    for (int i = 0; i < _fdsCounter; i++)
    {
        std::cout << YELLOW << i << " Connection events: " << _polls[i].events << RESET << std::endl;
        std::cout << YELLOW << i << " Connection fd: " << _polls[i].fd << RESET << std::endl;
        std::cout << YELLOW << i << " Connection revents: " << _polls[i].revents << RESET << std::endl;
    }
}
