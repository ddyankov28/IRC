/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/29 13:52:15 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

Server::Server(char *av1, char *av2) : _port(atoi(av1)), _password(av2), _usersCounter(0)
{}

Server::~Server()
{}

void    Server::setSockFd()
{
    _servSockFd = socket(PF_INET, SOCK_STREAM, 0); // get the fd for the server
    if (_servSockFd == -1)
    {
        perror("Error");
        throw std::runtime_error("Socket Error");
    }
    std::cout << GREEN << "Socket created successfully" << RESET << std::endl;
}

void    Server::setAddr()
{
    int opt = 1;
    if (setsockopt(_servSockFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
    {
        close(_servSockFd);
        perror("Error");
        throw std::runtime_error("Setting socket options Error");
    }
    memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;  // ipv4
    _servAddr.sin_addr.s_addr = INADDR_ANY; // accept connections on any of the available network interfaces on the machine
    _servAddr.sin_port = htons(_port); // convert port from host byte order to network byte order
    std::cout << GREEN << "Server configuration done successfully,Ready to bind" << RESET << std::endl;
}

void    Server::bindServ()
{
    if (bind(_servSockFd, (struct sockaddr *) &_servAddr, sizeof(_servAddr)) == -1) // bind to the port using the addr struct
    {
        close(_servSockFd);
        perror("Error");
        throw std::runtime_error("Binding Error");
    }
    std::cout << GREEN << "Server binded successfully" << RESET << std::endl;
}

void    Server::listenServ()
{
    if (listen(_servSockFd, MAX_CONNECTIONS) == -1)
    {
        close(_servSockFd);
        perror("Error");
        throw std::runtime_error("Listening Error");
    }
    std::cout << GREEN << "Server is listening successfully" << RESET << std::endl;
}

void    Server::acceptConnections()
{
    socklen_t _servAddrLen = sizeof(_servAddr);
    if ((_newSockFd = accept(_servSockFd, (struct sockaddr *) &_servAddr, &_servAddrLen)) == -1)
    {
        close(_servSockFd);
        throw std::runtime_error("ACCEPT ERROR");
    }
    std::cout << "Server accepted a connection" << std::endl;
    send(_newSockFd, "WELCOME TO THE SERVER: ", 24, 0);
    /* char buffer[100];
    recv(_newSockFd, buffer, 100, 0);
    size_t receivedBytes = strlen(buffer);
    if (receivedBytes > 0 && buffer[receivedBytes - 1] == '\n') {
        buffer[receivedBytes - 1] = '\0';
    }
    std::string userInput = buffer;
    std::cout << userInput << std::endl;
    std::cout << _password << std::endl;
    if (userInput == _password)
        std::cout << GREEN << "PASSWORD IS CORRECT" << RESET << std::endl;
    else
        std::cout << RED << "PASSWORD IS WRONG" << RESET << std::endl;*/
}

void    Server::setAndRunServ()
{
    int i = 0;
    
    setSockFd();
    setAddr();
    bindServ();
    listenServ();
    _polls[i].fd = _servSockFd;
    _polls[i].events = POLLIN;
    _polls[i].revents = 0;
    _usersCounter++;
    _shouldRun = true;
    std::cout << GREEN << "Server is running" << RESET << std::endl;
    while (_shouldRun)
    {
        int numEvents = poll(_polls, _usersCounter, -1);
        std::cout << numEvents << std::endl;
        if (numEvents < 0)
        {
            perror("Error:");
            throw std::runtime_error("Poll Error");
        }
        else if (numEvents > 0)
        {
            std::cout << "POLL ALREADY MONITORING" << std::endl;
            while (i < MAX_CONNECTIONS)
            {
                printFdStruct();
                std::cout << "in the Connections loop" << std::endl;
                if (_polls[i].revents == POLLIN)
                {
                    if (_polls[i].fd == _servSockFd)
                    {
                        std::cout << "Server ready to receive" << std::endl;
                        acceptConnections();
                    }
                }
                i++;
            }
        }
        else
            std::cout << "TIME OUT" << std::endl;
    }
}

void    Server::printFdStruct()
{
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        std::cout << YELLOW << i << " Connection events: " << _polls[i].events << RESET << std::endl;
        std::cout << YELLOW << i << " Connection fd: " << _polls[i].fd << RESET << std::endl;
        std::cout << YELLOW << i << " Connection revents: " << _polls[i].revents << RESET << std::endl;
    }
}
