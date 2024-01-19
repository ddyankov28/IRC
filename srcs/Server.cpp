/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/19 15:48:15 by ddyankov         ###   ########.fr       */
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

int Server::getPort()
{   return _port;   }

std::string Server::getPassword()
{   return _password;   }

int Server::getSockFd()
{   return _sockFd;  }

void    Server::bindServ()
{
    if (bind)
}
