/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/01 10:51:42 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

Server::Server(char *av1, char *av2) : _port(atoi(av1)), _password(av2), _fdsCounter(0)
{
    _creationTime = creationTime();
}

Server::~Server()
{
    for (unsigned int i = 0; i < _clients.size(); i++)
        delete _clients[i];
}

void    Server::setupErrorHandler(std::string msg)
{
    close(_serverFd);
    perror("Error");
    throw std::runtime_error(msg);
}
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
        setupErrorHandler("Setting socket options Error");
    memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;  // ipv4
    _servAddr.sin_addr.s_addr = INADDR_ANY; // accept connections on any of the available network interfaces on the machine
    _servAddr.sin_port = htons(_port); // convert port from host byte order to network byte order
    std::cout << GREEN << "Server configuration done successfully, Ready to bind" << RESET << std::endl;
}

void    Server::bindServ()
{
    if (bind(_serverFd, (struct sockaddr *) &_servAddr, sizeof(_servAddr)) == -1) // bind to the port using the addr struct
        setupErrorHandler("Binding Error");
    std::cout << GREEN << "Server binded successfully" << RESET << std::endl;
}

void    Server::listenServ()
{
    if (listen(_serverFd, MAX_CONNECTIONS) == -1)
        setupErrorHandler("Listening Error");
    std::cout << GREEN << "Server is listening successfully" << RESET << std::endl;
    _polls[0].fd = _serverFd;
    _polls[0].events = POLLIN | POLLOUT;
    _polls[0].revents = 0;
    _fdsCounter++;
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
    _fdsCounter++;
    std::cout << "Server accepted a connection" << std::endl;
    Client* newClient = new Client();
    newClient->setFd(newFd);
    newClient->setUserName("Deyan");
    _clients.push_back(newClient);
    static int num;
    std::cout << _clients[num]->getFd() << std::endl;
    std::cout << _clients[num]->getUserName() << std::endl;
    num++;
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
        int numEvents = poll(_polls, _fdsCounter, -1);
        if (numEvents < 0)
            setupErrorHandler("Poll Error");
        else if (numEvents > 0)
        {
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
                        if (_fdsCounter + 1 > MAX_CONNECTIONS)
                            throw std::out_of_range("Max Connections limit is reached");
                        acceptAndAddConnections();
                    }
                    /*else        // it is just a regular client
                    {
                        std::cout << "Message from Client: " << std::endl;
                        int bytes = recv(_polls[i].fd, _buffer, sizeof(_buffer) - 1, 0);
                        write(1, &_buffer, bytes);
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
                        else // We got some good data from a client
                        { 
                            for (int j = 0; j < _fdsCounter; j++) // send everyone
                            {
                                int destFd = _polls[j].fd;
                                // Except the listener and ourselves
                                if (destFd != senderFd && destFd != _serverFd)
                                {
                                    if (int bytesSend = send(destFd, _buffer, sizeof(_buffer), 0) == -1)
                                        perror("Error:");
                                }
                                        
                            }
                        }
                    }*/
                  //  printFdStruct();
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
