/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/22 15:56:01 by ddyankov         ###   ########.fr       */
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
        throw std::out_of_range("SOCKET_ERROR");
}

void    Server::bindServ()
{
    if (bind(_sockFd, (struct sockaddr *) &_servAddr, sizeof(_servAddr)) == -1)
    {
        close(_sockFd);
        perror("ERROR");
        throw std::runtime_error("BINDING ERROR");
    }
}

void    Server::setAddr()
{
    memset(&_servAddr, 0, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = INADDR_ANY;
    _servAddr.sin_port = htons(_port);
}

void    Server::setServ()
{
    setSockFd();
    setAddr();
    bindServ();
}
