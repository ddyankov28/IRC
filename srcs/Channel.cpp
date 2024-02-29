/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vstockma <vstockma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 14:31:17 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/29 16:29:30 by vstockma         ###   ########.fr       */
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

Channel&  Channel::operator=(const Channel& inst)
{
    _name = inst._name;
    _server = inst._server;
    _operators = inst._operators;
    _members = inst._members;
    _topic = inst._topic;
    _isInviteChannel = inst._isInviteChannel;
    _isTopicRestricted = inst._isTopicRestricted;
    _channelKey = inst._channelKey;
    _isKeyChannel = inst._isKeyChannel;
    _limit = inst._limit;
    return *this;
}

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

void    Channel::setisInviteChannel(char c)
{
    if (c == '-')
        _isInviteChannel = false;
    else
        _isInviteChannel = true;
}

void    Channel::setisTopicRestricted(char c)
{
    if (c == '-')
        _isTopicRestricted = false;
    else
        _isTopicRestricted = true;
}

void    Channel::setlimit(char c, std::string limit)
{
    std::cout << c << std::endl;
    std::cout << "<"<< limit << ">" << std::endl;
    if (c == '-')
        _limit = 0;
    else
        _limit = atoi(limit.c_str());
}

void    Channel::setKeyChannel(char c, std::string key)
{
    if (c == '-')
        _channelKey = "";
    else
        _channelKey = key;
}


