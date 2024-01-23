/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/23 14:59:19 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

Server::Server(char *av1, char *av2) : _port(atoi(av1)), _password(av2)
{}

Server::~Server()
{}

void    Server::setSockFd()
{
    _sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockFd == -1)
    {
        perror("ERROR");
        throw std::out_of_range("SOCKET ERROR");
    }
    std::cout << GREEN << "Socket created successfully" << RESET << std::endl;
}

void    Server::setAddr()
{
    int opt = 1;
    if (setsockopt(_sockFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) 
    {
        perror("ERROR");
        throw std::runtime_error("SET OPTIONS ERROR");
    }
    memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;  // ipv4
    _servAddr.sin_addr.s_addr = INADDR_ANY; // accept connections on any of the available network interfaces on the machine
    _servAddr.sin_port = htons(_port); // convert port from host byte order to network byte order
    std::cout << GREEN << "Server configuration done successfully,Ready to bind" << RESET << std::endl;
}

void    Server::bindServ()
{
    if (bind(_sockFd, (struct sockaddr *) &_servAddr, sizeof(_servAddr)) == -1)
    {
        close(_sockFd);
        perror("ERROR");
        throw std::runtime_error("BINDING ERROR");
    }
    std::cout << GREEN << "Server binded successfully" << RESET << std::endl;
}

void    Server::listenServ()
{
    if (listen(_sockFd, MAX_CONNECTIONS) == -1)
    {
        close(_sockFd);
        perror("ERROR");
        throw std::runtime_error("LISTEN ERROR");
    }
    std::cout << GREEN << "Server is listening successfully" << RESET << std::endl;
}

void    Server::acceptConnections()
{
    socklen_t _servAddrLen = sizeof(_servAddr);
    if ((_newSockFd = accept(_sockFd, (struct sockaddr *) &_servAddr, &_servAddrLen)) == -1)
    {
        close(_sockFd);
        perror("ERROR");
        throw std::runtime_error("ACCEPT ERROR");
    }
}

void    Server::setServ()
{
    setSockFd();
    setAddr();
    bindServ();
    listenServ();
    acceptConnections();
    while (1)
    {
        char buffer[1024] = { 0 };
        char line[1024] = { 0 };
        ssize_t valRead = read(_newSockFd, buffer, sizeof(buffer) -1);
        (void)valRead;
        std::cout << buffer << std::endl;
        ssize_t inRead = read(0, line, sizeof(line) - 1);
        (void)inRead;
        send(_newSockFd, line, sizeof(line), 0);
        std::cout << GREEN << "Message sent" << RESET << std::endl;
        if (!strcmp(buffer, "Bye")) 
            break;
    }
    close(_newSockFd);
    close(_sockFd);
}
