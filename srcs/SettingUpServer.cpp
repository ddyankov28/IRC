/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SettingUpServer.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:38:21 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/01 13:45:56 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

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
        throw std::runtime_error("Socket Error");
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
