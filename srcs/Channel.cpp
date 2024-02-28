/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 14:31:17 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/28 13:24:11 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Channel.hpp"

Channel::Channel(std::string name, Server& server) : _name(name), _server(server), _topic(""), _isInviteChannel(false), _isTopicRestricted(false), _channelKey(""), _isKeyChannel(false), _limit(0)
{
    (void)_server;
    (void)_isTopicRestricted;
    (void)_limit;
}

Channel::~Channel()
{}

std::string Channel::getChannelName()
{
    return _name;
}

bool    Channel::getisInviteChannel()
{
    return _isInviteChannel;
}

bool    Channel::getisKeyChannel()
{
    return _isKeyChannel;
}

std::vector<Client *>&    Channel::getMembers()
{
    return _members;
}

std::vector<Client *>&    Channel::getOperators()
{
    return _operators;
}

Client* Channel::getMemberByNick(std::string Nick)
{
    std::vector<Client *>::iterator it = _members.begin();
    while (it != _members.end())
    {
        if ((*it)->getNickName() == Nick)
            return *it;
        it++;
    }   
    return NULL;
}

Client* Channel::getOpByNick(std::string Nick)
{
    std::vector<Client *>::iterator it = _operators.begin();
    while (it != _operators.end())
    {
        if ((*it)->getNickName() == Nick)
            return *it;
        it++;
    }   
    return NULL;
}
