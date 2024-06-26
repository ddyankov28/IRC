/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 14:31:17 by ddyankov          #+#    #+#             */
/*   Updated: 2024/03/06 16:40:32 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Channel.hpp"

Channel::Channel(std::string name, Server& server) : _name(name), _server(server), _topic(""), _isInviteChannel(false), _isTopicRestricted(false), _channelKey(""), _limit(0)
{}

Channel::~Channel()
{}

Channel&  Channel::operator=(const Channel& inst)
{
    _name = inst._name;
    _server = inst._server;
    _operators = inst._operators;
    _members = inst._members;
    _invitedUsers = inst._invitedUsers;
    _topic = inst._topic;
    _isInviteChannel = inst._isInviteChannel;
    _isTopicRestricted = inst._isTopicRestricted;
    _channelKey = inst._channelKey;
    _limit = inst._limit;
    return *this;
}

std::string Channel::getChannelName() const
{ return _name; }

bool    Channel::getisInviteChannel() const
{ return _isInviteChannel; }

bool    Channel::getisTopicRestricted() const
{ return _isTopicRestricted; }

std::string Channel::getTopic() const
{ return _topic; }

int Channel::getLimit() const
{ return _limit; }

std::string Channel::getchannelKey() const
{ return _channelKey; }

std::vector<Client *>&    Channel::getMembers()
{ return _members; }

std::vector<Client *>&    Channel::getOperators()
{ return _operators; }

std::vector<std::string>&    Channel::getinvitedUsers()
{ return _invitedUsers; }

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

void    Channel::setTopic(std::string newTopic)
{
    _topic = newTopic;
}

void    Channel::setlimit(char c, std::string limit)
{
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

void    Channel::setOperator(char c, std::string Nickname)
{
    try
    {
        if (c == '-')
        {
            std::vector<Client *>::iterator itOperators = _operators.begin();
            while (itOperators != _operators.end())
            {
                if ((*itOperators)->getNickName() == Nickname)
                    _operators.erase(itOperators);
                else
                    ++itOperators;
            }
        }
        else
        {
            if (getMemberByNick(Nickname) == NULL || getOpByNick(Nickname) != NULL)
                return ;
            Client* newOp = _server.getClientByNick(Nickname);
            _operators.push_back(newOp);
        }
    }
    catch(const std::exception& e)
    {
        return ;
    } 
}

int Channel::UserIsInvited(std::string Nick, int sw)
{
    std::vector<std::string>::iterator it;
    for (it = _invitedUsers.begin(); it != _invitedUsers.end(); ++it)
    {
        if (*it == Nick)
        {  
            if (sw == 1)
                _invitedUsers.erase(it);
            return 1;
        }
    }
    return 0;
}
