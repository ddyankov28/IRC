/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:04:57 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/01 14:54:50 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Client.hpp"

Client::Client() : _userName(""), _nickName(""), _cliFd(0)
{}

Client::~Client()
{}

void    Client::setFd(int pollFd)
{
    _cliFd = pollFd;
}

void    Client::setUserName(std::string userName)
{
    _userName = userName;
}

int Client::getFd()
{
    return _cliFd;
}

std::string Client::getUserName()
{
    return _userName;
}
