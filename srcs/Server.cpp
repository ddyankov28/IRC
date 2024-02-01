/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/01 15:17:56 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

// Constructor
Server::Server(char *av1, char *av2) : _port(atoi(av1)), _password(av2), _fdsCounter(0)
{
    _creationTime = creationTime();
}

// Destructor
Server::~Server()
{
    for (unsigned int i = 0; i < _clients.size(); i++)
        delete _clients[i];
}

void    Server::acceptAndAddConnections()
{
    int newFd;
    struct sockaddr_in  newConnection;
    socklen_t newConnectionLen = sizeof(newConnection);
    if ((newFd = accept(_serverFd, (struct sockaddr *) &newConnection, &newConnectionLen)) == -1)
        setupErrorHandler("Accepting a new connection Error");
    _polls[_fdsCounter].fd = newFd;
    _polls[_fdsCounter].events = POLLIN | POLLOUT;
    _polls[_fdsCounter].revents = 0;
    send(_polls[_fdsCounter].fd, "---Welcome to the 42_IRC Server---\ncreated on: ", 48, 0);
    send(_polls[_fdsCounter].fd, _creationTime.c_str(), _creationTime.size(), 0);
    send(_polls[_fdsCounter].fd, REGISTER, sizeof(REGISTER), 0);
    _fdsCounter++;
    std::cout << "Server accepted a connection" << std::endl;
    Client* newClient = new Client();
    newClient->setFd(newFd);
    newClient->setUserName("Deyan");
    _clients.push_back(newClient);
}

void    Server::handleEvents()
{
    // Check existing connections looking for data ro read //
    for (int i = 0; i < _fdsCounter; i++)
    {
        // Check if someone is ready to read //
        if (_polls[i].revents & POLLIN)
        {
            // If it is the server, we handle the new connection //
            if (_polls[i].fd == _serverFd)
            {
                std::cout << GREEN << "Server ready to accept" << RESET << std::endl;
                if (_fdsCounter + 1 > MAX_CONNECTIONS)
                    throw std::out_of_range("Max Connections limit is reached");
                acceptAndAddConnections();
            }
            else // It is client //
            {
                std::cout << "Message from Client: " << std::endl;
                int bytes = recv(_polls[i].fd, _buffer, sizeof(_buffer) - 1, 0);
                _buffer[bytes] = '\0';
                std::string cliMsg = _buffer;
                std::cout << cliMsg << cliMsg.size() << std::endl;
                int senderFd = _polls[i].fd;
                // Connection closed or Error
                if (bytes <= 0)
                {
                    if (!bytes)
                        std::cout << RED << "Connection " << senderFd << " was closed" << RESET << std::endl;   
                    else
                        perror("Error");
                    close(_polls[i].fd);
                }
                /*else // We got some good data from a client
                { 
                    for (size_t j = 0; j < _clients.size(); j++)
                    {
                        int destFd = _clients[j]->getFd();
                        if (destFd != senderFd && destFd != _serverFd)
                        {
                            int bytesSend = send(destFd, _buffer, sizeof(_buffer) - 1, 0);
                            if (bytesSend == -1)
                                perror("Error:");
                        }
                    }
                }*/
            }
        }
    }
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
        int numEvents = poll(_polls, _fdsCounter, 3000);
        if (numEvents < 0)
            setupErrorHandler("Poll Error");
        else if (numEvents > 0)
            handleEvents();
    }
}

void    Server::printFdStruct()
{
    for (int i = 0; i < _fdsCounter; i++)
    {
        std::cout << YELLOW << i << " Connection events: " << _polls[i].events << RESET << std::endl;
        std::cout << YELLOW << i << " Connection fd: " << _polls[i].fd << RESET << std::endl;
        std::cout << YELLOW << i << " Connection revents: " << _polls[i].revents << RESET << std::endl;
        LINE;
    }
}

std::string Server::creationTime()
{
    time_t      rawtime;
    struct tm*  timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}
