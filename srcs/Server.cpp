/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/23 12:45:07 by ddyankov         ###   ########.fr       */
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

void    Server::setServ()
{
    setSockFd();
    setAddr();
    bindServ();
    listenServ();
}
