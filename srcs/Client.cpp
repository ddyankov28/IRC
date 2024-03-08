/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:04:57 by ddyankov          #+#    #+#             */
/*   Updated: 2024/03/07 11:07:53 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Client.hpp"

Client::Client(int cliFd, Server& server) : _fd(cliFd), _userName(""), _nickName(""), _command(""), _buff(""), _isRegistered(false), _passIsCorrect(false), _registerSteps(0), _server(server)
{}

Client::~Client()
{}

int Client::getFd() const
{ return _fd; }

std::string Client::getIp() const
{ return _ip; }

std::string Client::getUserName() const
{ return _userName; }

std::string Client::getNickName() const
{ return _nickName; }

std::string Client::getCliCommand() const
{ return _command; }

std::string Client::getBuff() const
{ return _buff; }

bool    Client::getIsRegistered() const
{ return _isRegistered; }

std::vector<std::string> Client::getSplitMoreLines() const
{ return _splitMoreLines; }

void    Client::setFd(int pollFd)
{ _fd = pollFd; }

void    Client::setIp(std::string ip)
{ _ip = ip; }

void    Client::setUserName(std::string userName)
{
    if (_userName != "")
    {
        std::string msg = ":42_IRC 462 " + _userName + " USER :You may not reregister\n";
        send(_fd, msg.c_str(), msg.size(), 0);
    }
    _userName = userName;
}

void    Client::setNickName(std::string& nickName)
{
    if (_nickName != "")
    {
        std::string msg = ":" + _nickName + "!~ " + _splitedCommand[0] + " :" + _splitedCommand[1] + "\n";
        send(_fd, msg.c_str(), msg.size(), 0);
    }
    _nickName = nickName;
}

void    Client::setCliCommand(std::string msg)
{ _command = msg; }

void    Client::setPassword(std::string pass)
{ _password = pass; }

void    Client::setBuff(std::string add)
{ _buff = add; }

void    Client::splitCommand()
{
    std::istringstream  iss(_command);
    std::string         word;
    
    while (iss >> word)
        _splitedCommand.push_back(word);
    /*std::vector<std::string>::iterator it = _splitedCommand.begin();
    while (it != _splitedCommand.end())
    {
        std::cout << *it << std::endl;
        it++;
    }
    std::cout << _splitedCommand.size() << std::endl;*/
}

void    Client::checkCommand()
{
    if (_splitedCommand.size() < 1)
        return;
    if (_splitedCommand.size() == 1 && isValidCommand())
    {
        needMoreParams();
        return ;
    }
    if (((_splitedCommand[0] == "PASS" && _passIsCorrect == false) || _splitedCommand[0] == "NICK" || _splitedCommand[0] == "USER") && _splitedCommand.size() >= 2)
    {
        if (_splitedCommand[0] == "PASS" && _splitedCommand[1] != _password)
            send(_fd, "⛔️Password is incorrect⛔️\n", 34, 0);
        else if (_splitedCommand[0] == "PASS" && _splitedCommand.size() > 2)
        {
            send(_fd, "PASS :Too much parameters\n", 26, 0);
                return ;
        }
        else if (_splitedCommand[0] == "PASS" && _splitedCommand[1] == _password)
        {
            _registerSteps++;
            _passIsCorrect = true;
        }
        else if (_splitedCommand[0] == "NICK")
        {
            if (_splitedCommand.size() > 2)
            {
                send(_fd, "NICK :Too much parameters\n", 26, 0);
                return ;
            }         
            else if (_server.getClientByNick(_splitedCommand[1]))
            {
                std::string msg = _splitedCommand[1] + " :Nickname already in use\n";
                send(_fd, msg.c_str(), msg.size(), 0);
                return ;
            }
            if (_nickName.empty())
                _registerSteps++;
            setNickName(_splitedCommand[1]);
        }
        else if (_splitedCommand[0] == "USER")
        {
            if (_splitedCommand.size() > 2 && _splitedCommand[2] != "0")
            {
                send(_fd, "USER :Too much parameters\n", 26, 0);
                return ;
            }       
            else if (_server.getClientByUser(_splitedCommand[1]))
            {
                std::string msg = _splitedCommand[1] + " :Username already in use\n";
                send(_fd, msg.c_str(), msg.size(), 0);
                return ;
            }
            if (_userName.empty())
                _registerSteps++;
            setUserName(_splitedCommand[1]);
        }
    }
    else if (_splitedCommand[0] == "PASS" && _passIsCorrect == true)
        send(_fd, "✅You already provided a correct password✅\n", 46, 0);
    else if (_isRegistered == false && _splitedCommand.size() > 0)
    {
        if (_splitedCommand.size() == 1)
            send(_fd, "🚫Wrong command, you have to register yourself🚫\n", 53, 0);
        else if (_splitedCommand[0] != "CAP" && _splitedCommand[1] != "LS")
            send(_fd, "🚫Wrong command, you have to register yourself🚫\n", 53, 0);
    }
    if (_registerSteps == 3)
    {
        _isRegistered = true;
        _registerSteps++;
        send(_fd, "✅You are already registered✅\n", 33, 0);
    }
}

void    Client::checkFeatures()
{
    if (_splitedCommand.size() < 1)
        return;
    if (!_isRegistered)
    {
        _splitedCommand.erase(_splitedCommand.begin(), _splitedCommand.end());
        return ;
    }
    if (_splitedCommand[0] == "PRIVMSG" && _splitedCommand.size() == 1)
    {
        std::string msg = ERR_NORECIPIENT + _nickName + ERR_NORECIP;
        msg += "(" +_splitedCommand[0] + ")\n";
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (_splitedCommand[0] == "PRIVMSG")
        privmsg();
    else if (_splitedCommand[0] == "JOIN")
        joinChannels();
    else if (_splitedCommand[0] == "KICK")
        kickUsers();
    else if (_splitedCommand[0] == "INVITE")
        inviteUsers();
    else if (_splitedCommand[0] == "TOPIC")
        changeTopic();
    else if (_splitedCommand[0] == "MODE")
    {
        if (startMode() == 1) return ;
    }
    else if (_splitedCommand[0] != "NICK" && _splitedCommand[0] != "USER" && _splitedCommand[0] != "PASS" && _isRegistered == true)
    {
        std::string msg = _splitedCommand[0] + " :Command is not valid\n";
        (void)msg;
    }
    _splitedCommand.erase(_splitedCommand.begin(), _splitedCommand.end());
}

void    Client::privmsg()
{
    int isChannel = 0;
    Client* Reciever = _server.getClientByNick(_splitedCommand[1]);
    try
    {
        Channel& RecieverChannel = _server.getChannelbyName(_splitedCommand[1]);
        (void)RecieverChannel;
    }
    catch(const std::exception& e)
    {
        isChannel = 1;
    }
    if (!Reciever && isChannel)
    {
        std::string msg = _splitedCommand[1] + ERR_NOSUCHNICK;
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (_splitedCommand.size() == 2)
    {
        std::string msg = ERR_NOTEXTTOSEND;
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (_splitedCommand[2][0] != ':')
        send(getFd(), ":Wrong Format of Message\n", 25, 0);
    else if(isChannel == 0)
    {
        Channel& RecieverChannel = _server.getChannelbyName(_splitedCommand[1]);
        if (ClientInChannel(RecieverChannel))
            sendMsgInChannel(RecieverChannel);
        else
        {
            std::string msg = _splitedCommand[1] + ERR_NOTONCHANNEL;
            send(_fd, msg.c_str(), msg.size(), 0);
        }
    }
    else
    {
        std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " ";
        send(Reciever->getFd(), msg.c_str(), msg.size(), 0);
        size_t i = 2;
        while (i < _splitedCommand.size())
        {
            send(Reciever->getFd(), _splitedCommand[i].c_str() , _splitedCommand[i].size(), 0);
            if (i < _splitedCommand.size() - 1)
                send(Reciever->getFd(), " ", 1, 0);
            i++;
        }
        send(Reciever->getFd(), "\n", 1, 0);
    }
}

void    Client::joinChannels()
{
    if (_splitedCommand[1][0] == '#')
    {
        try
        {
            Channel& currentChannel =_server.getChannelbyName(_splitedCommand[1]);
            if ((int)currentChannel.getMembers().size() >= currentChannel.getLimit() && currentChannel.getLimit() != 0)
            {
                std::string msg = ":42_IRC 471 " + _nickName + _splitedCommand[1] + " :Cannot join channel (+l)\n";
                send(_fd, msg.c_str(), msg.size(), 0);
            }  
            else if (currentChannel.getisInviteChannel())
            {
                if (currentChannel.getMemberByNick(_nickName))
                    return ;
                else if (currentChannel.UserIsInvited(_nickName, 1) == 0)
                {
                    std::string msg = ":42_IRC 473 " + _nickName + _splitedCommand[1] + ERR_INVITEONLYCHAN;
                    send(getFd(), msg.c_str(), msg.size(), 0);
                }
                else
                    join(currentChannel);
            }
            else if (!currentChannel.getchannelKey().empty())
            {
                if (currentChannel.UserIsInvited(_nickName, 1) == 1)
                {
                    join(currentChannel);
                    return ;
                }
                if (_splitedCommand.size() != 3)
                {
                    std::string msg = ":42_IRC 475 " + _nickName + _splitedCommand[1] + " :Cannot join channel (+k)\n";
                    send(getFd(), msg.c_str(), msg.size(), 0);
                    return ;
                }
                if (_splitedCommand[2] == currentChannel.getchannelKey())
                    join(currentChannel);
                else
                {
                    std::string msg = ":42_IRC 475 " + _nickName + _splitedCommand[1] + " :Cannot join channel (+k)\n";
                    send(getFd(), msg.c_str(), msg.size(), 0);
                }
            }  
            else if (currentChannel.getchannelKey().empty())
            {
                if (currentChannel.getMemberByNick(_nickName))
                    return ;
                currentChannel.UserIsInvited(_nickName, 1);
                join(currentChannel);
            } 
        }
        catch (std::exception& e)
        {
            Channel newChannel(_splitedCommand[1], _server);

            newChannel.getMembers().push_back(this);
            newChannel.getOperators().push_back(this);
            _server.getChannels().push_back(newChannel);
            std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + "\n";
            send(getFd(), msg.c_str(), msg.size(), 0);
            msg = ":42_IRC 353 " + getNickName() + " = "  + _splitedCommand[1] + " :@" + getNickName() + "\n";
            send(getFd(), msg.c_str(), msg.size(), 0);
            msg = ":42_IRC 366 " + getNickName() + " " + _splitedCommand[1] + " :End of NAMES list\n";
            send(getFd(), msg.c_str(), msg.size(), 0);
        }          
    }
}

void    Client::join(Channel& currentChannel)
{
    currentChannel.getMembers().push_back(this);
    std::vector<Client *>::iterator it = currentChannel.getMembers().begin();
    std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + "\n";
    sendToAllMembers(currentChannel, msg);
    msg = ":42_IRC 353 " + getNickName() + " = "  + _splitedCommand[1] +  " :";
    send(getFd(), msg.c_str(), msg.size(), 0);
    it = currentChannel.getMembers().begin();
    while (it != currentChannel.getMembers().end())
    {
        if (currentChannel.getOpByNick((*it)->getNickName()))
            msg = " @" + (*it)->getNickName();
        else
            msg = " " + (*it)->getNickName();
        send(getFd(), msg.c_str(), msg.size(), 0);
        it++;
    }
    send(getFd(), "\n", 1, 0);
    msg = ":42_IRC 366 " + getNickName() + " " + _splitedCommand[1] + " :End of NAMES list\n";
    send(getFd(), msg.c_str(), msg.size(), 0);
    if (!currentChannel.getTopic().empty())
    {
        msg = ":42_IRC 332 " + _splitedCommand[1] + " " + currentChannel.getTopic() + "\n";
        send (_fd, msg.c_str(), msg.size(), 0);
    }
}

int     Client::kick(Channel& currentChannel, std::vector<Client *>::iterator itMembers)
{
    if ((*itMembers)->getNickName() == _splitedCommand[2])
    {
        std::vector<Client *>::iterator it = currentChannel.getMembers().begin();
        while (it != currentChannel.getMembers().end())
        {
            std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " " + _splitedCommand[2] + " :" + _nickName + "\n";
            send((*it)->getFd(), msg.c_str(), msg.size(), 0);
            it++;
        }
        currentChannel.getMembers().erase(itMembers);
        std::vector<Client *>::iterator itOperators = currentChannel.getOperators().begin();
        while (itOperators != currentChannel.getOperators().end())
        {
            if ((*itOperators)->getNickName() == _splitedCommand[2])
                currentChannel.getOperators().erase(itOperators);
            else
                ++itOperators;
        }
        if (currentChannel.getMembers().size() == 0)
        {
            std::vector<Channel>::iterator it = _server.getChannels().begin();
            while (it != _server.getChannels().end())
            {
                if (it->getChannelName() == _splitedCommand[1])
                    _server.getChannels().erase(it);
                else
                    ++it;
            }
        }
        return 1;
    }
    return 0;
}

void    Client::kickUsers()
{
    if (_splitedCommand.size() == 1)
        return ;
    try
    {
        Channel& currentChannel = _server.getChannelbyName(_splitedCommand[1]);
        if (ClientInChannel(currentChannel))
        {
            std::vector<Client *>::iterator it = currentChannel.getOperators().begin();
            while (it != currentChannel.getOperators().end())
            {
                if ((*it)->getNickName() == _nickName) //you are operator
                {
                    std::vector<Client *>::iterator itMembers = currentChannel.getMembers().begin();
                    while (itMembers != currentChannel.getMembers().end())
                    {
                        if (kick(currentChannel, itMembers) == 1)
                            return ;
                        else
                            ++itMembers;   
                    }
                    std::string msg = ":42_IRC 441 " + _splitedCommand[2] + " " + _splitedCommand[1] + " :They aren't on that channel\n";
                    send(_fd, msg.c_str(), msg.size(), 0);
                    return ;
                }
                it++;
            }
            std::string msg = ":42_IRC 482 " + _nickName + " " + _splitedCommand[1] + " :You're not channel operator\n";
            send(_fd, msg.c_str(), msg.size(), 0);  
        }
        else
        {
            std::string msg = _splitedCommand[1] + ERR_NOTONCHANNEL;
            send(_fd, msg.c_str(), msg.size(), 0);
        }  
    }
    catch (std::exception& e)
    {
        //send(_fd, _splitedCommand[1].c_str(), _splitedCommand[1].size(), 0);
        send(_fd, " :No such channel\n", 18, 0);
    }
}

void    Client::inviteUsers()
{
    if (_splitedCommand.size() == 1)
        return ;
    else if (_splitedCommand.size() == 2)
    {
        std::string msg = ":42_IRC 461 " + _nickName + " " + _splitedCommand[0] + ERR_NEEDMOREPARAMS;
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else
    {
        if (!_server.getClientByNick(_splitedCommand[1]))
        {
            std::string msg = ":42_IRC 401 " + _nickName + " " + _splitedCommand[2] + " " + _splitedCommand[1] + ERR_NOSUCHNICK;
            send(getFd(), msg.c_str(), msg.size(), 0);
            return ;
        }
        try
        {
            Channel& currentChannel = _server.getChannelbyName(_splitedCommand[2]);
            inviteOnChannel(currentChannel);
        }
        catch(const std::exception& e)
        {
            std::string msg = ":42_IRC 401 " + _nickName + " " + _splitedCommand[2] + ERR_NOSUCHNICK;
            send(getFd(), msg.c_str(), msg.size(), 0);
            return ;
        }
        
    }
}

void    Client::inviteOnChannel(Channel& currentChannel)
{
    if (!currentChannel.getOpByNick(getNickName()))
    {
        std::string msg = ":42_IRC 482 " + _nickName + " " + _splitedCommand[2] + " :You're not channel operator\n";
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (currentChannel.UserIsInvited(_splitedCommand[1], 0) == 1)
        return ;
    else if (currentChannel.getMemberByNick(_splitedCommand[1]) != NULL)
    {
        std::string msg = ":42_IRC 443 " + _nickName + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " " + _splitedCommand[2] + " :is already on channel\n";
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else
    {
        currentChannel.getinvitedUsers().push_back(_splitedCommand[1]);
        std::string msg = ":42_IRC 341 " + _nickName + " " + _splitedCommand[1] + " " + _splitedCommand[2] + "\n";
        msg += ":42_IRC NOTICE @" + _splitedCommand[2] + " :" + _nickName + " invited " + _splitedCommand[1] + " into channel " + _splitedCommand[2] + "\n";
        send(getFd(), msg.c_str(), msg.size(), 0);
        Client *receiver = _server.getClientByNick(_splitedCommand[1]);
        msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " :" + _splitedCommand[2] + "\n";
        send(receiver->getFd(), msg.c_str(), msg.size(), 0);
        
    }
}

void    Client::changeTopic()
{
    if (_splitedCommand.size() == 1)
        return ;
    else
    {
        try
        {
            Channel& currentChannel = _server.getChannelbyName(_splitedCommand[1]);
            if (_splitedCommand.size() == 2)
            {
                if (currentChannel.getTopic().empty())
                {
                    std::string msg = ":42_IRC 331 " + getNickName() + " " + _splitedCommand[1] + " :No topic is set\n";
                    send(_fd, msg.c_str(), msg.size(), 0); 
                }
                else
                {
                    std::string msg = ":42_IRC 332 " + getNickName() + " " + _splitedCommand[1] + " " + currentChannel.getTopic() + "\n";
                    send(_fd, msg.c_str(), msg.size(), 0);
                }
            }
            else if (_splitedCommand.size() != 3)
                return ;
            else if (!currentChannel.getisTopicRestricted())
            {
                currentChannel.setTopic(_splitedCommand[2]);
                std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " :" + _splitedCommand[2] + "\n";
                sendToAllMembers(currentChannel, msg);
            }
            else
            {
                if (!currentChannel.getOpByNick(getNickName()))
                {
                    std::string msg = ":42_IRC 482 " + _nickName + " " + _splitedCommand[1] + " :You're not channel operator\n";
                    send(getFd(), msg.c_str(), msg.size(), 0);
                }
                else
                {
                    currentChannel.setTopic(_splitedCommand[2]);
                    std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " :" + _splitedCommand[2] + "\n";
                    sendToAllMembers(currentChannel, msg);
                }
            }
        }
        catch(const std::exception& e)
        {
            std::string msg = _splitedCommand[2] + ERR_NOSUCHNICK;
            send(getFd(), msg.c_str(), msg.size(), 0);
            return ;
        }
        
    }
}

int    Client::startMode()
{
    if (_splitedCommand.size() == 1 || _splitedCommand.size() == 2)
    {
        _splitedCommand.erase(_splitedCommand.begin(), _splitedCommand.end()); 
        return 1;
    }
    else if (isChannelOperator())
        handleMode();
    else
    {
        std::string msg = ":42_IRC 482 " + _nickName + " " + _splitedCommand[1] + " :You're not channel operator\n";
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    return 0;
}

void    Client::handleMode()
{
    if (_splitedCommand.size() == 3 || _splitedCommand.size() == 4)
    {
        if (_splitedCommand[2].size() != 2)
        {
            std::string msg = ":42_IRC 472 "+ _nickName + " " + _splitedCommand[1] + " " + _splitedCommand[2] + ERR_UNKNOWNMODE;
            send(getFd(), msg.c_str(), msg.size(), 0);
        }
        else if (_splitedCommand[2][0] == '-' || _splitedCommand[2][0] == '+')
        {
            if (_splitedCommand[2][1] == 'i' || _splitedCommand[2][1] == 't')
                handleIandT();
            else if (_splitedCommand[2][1] == 'o' || _splitedCommand[2][1] == 'l' || _splitedCommand[2][1] == 'k')
                handleFourParams();
            else
            {
                std::string msg = ":42_IRC 472 "+ _nickName  + " " + _splitedCommand[1] + " " + _splitedCommand[2] + ERR_UNKNOWNMODE;
                send(getFd(), msg.c_str(), msg.size(), 0);
            }
        }
        else
        {
            std::string msg = ":42_IRC 472 "+ _nickName + " " + _splitedCommand[1] + " " + _splitedCommand[2] + ERR_UNKNOWNMODE;
            send(getFd(), msg.c_str(), msg.size(), 0);
        }
    }
}

void    Client::handleIandT()
{
    if (_splitedCommand.size() == 4)
        return ;
    else if (_splitedCommand[2][1] == 'i')
    {
        try {
            Channel& currentChannel = _server.getChannelbyName(_splitedCommand[1]);
            currentChannel.setisInviteChannel(_splitedCommand[2][0]);
            std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " " + _splitedCommand[2] + "\n";
            sendToAllMembers(currentChannel, msg);
        }
        catch(const std::exception& e)
        { return ; }
    }
    else
    {
        try {
            Channel& currentChannel = _server.getChannelbyName(_splitedCommand[1]);
            currentChannel.setisTopicRestricted(_splitedCommand[2][0]);
            std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " " + _splitedCommand[2] + "\n";
            sendToAllMembers(currentChannel, msg);
        }
        catch(const std::exception& e)
        { return ; }
    }
}

void    Client::handleFourParams()
{
    if (_splitedCommand[2][1] == 'l')
        handleLimit();
    else if (_splitedCommand[2][1] == 'k')
        handleKeyChannel();
    else
    {
        if (_splitedCommand.size() != 4)
        {
            send(_fd, ":Not enough parameters\n", 23, 0);
            return ;
        }
        try
        {
            Channel& currentChannel = _server.getChannelbyName(_splitedCommand[1]);
            currentChannel.setOperator(_splitedCommand[2][0], _splitedCommand[3]);
            std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " " + _splitedCommand[2] + " " + _splitedCommand[3] + "\n";
            sendToAllMembers(currentChannel, msg);
        }
        catch(const std::exception& e)
        {
            return ;
        }
    }
}

void    Client::handleLimit()
{
    try
    {
        Channel& currentChannel = _server.getChannelbyName(_splitedCommand[1]);
        if (_splitedCommand[2] == "-l")
        {
            if (_splitedCommand.size() != 3)
                return ;
            currentChannel.setlimit(_splitedCommand[2][0], "");
            std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " " + _splitedCommand[2] + "\n";
            sendToAllMembers(currentChannel, msg);
            return ;
        }
        else if (_splitedCommand.size() != 4)
            return ;
        else if (!stringHasOnlyDigits())
        {
            std::string msg = ":42_IRC 472 "+ _nickName  + " " + _splitedCommand[1] + " : +l needs a number as an argument\n";
            send(_fd, msg.c_str(), msg.size(), 0);
            return ;
        }
        else if (atoi(_splitedCommand[3].c_str()) > MAX_CONNECTIONS)
        {
            std::string msg = ":42_IRC 472 "+ _nickName  + " " + _splitedCommand[1] + " :limit exceeds allowed max connections\n";
            send(_fd, msg.c_str(), msg.size(), 0);
            return ;
        }
        else if (atoi(_splitedCommand[3].c_str()) < 1)
        {
            std::string msg = ":42_IRC 472 "+ _nickName  + " " + _splitedCommand[1] + " :limit has to be at least 1\n";
            send(_fd, msg.c_str(), msg.size(), 0);
            return ;
        }
        currentChannel.setlimit(_splitedCommand[2][0], _splitedCommand[3]);
        std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " " + _splitedCommand[2] + " " + _splitedCommand[3] + "\n";
        sendToAllMembers(currentChannel, msg);
    }
    catch(const std::exception& e)
    {
        return ;
    }
}

void    Client::handleKeyChannel()
{
    try
    {
        Channel& currentChannel = _server.getChannelbyName(_splitedCommand[1]);
        if (_splitedCommand[2] == "-k")
        {
            currentChannel.setKeyChannel(_splitedCommand[2][0], "");
            std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " " + _splitedCommand[2] + "\n";
            sendToAllMembers(currentChannel, msg);
            return ;
        }
        else if (_splitedCommand.size() != 4)
            return ;
        currentChannel.setKeyChannel(_splitedCommand[2][0], _splitedCommand[3]);
        std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " " + _splitedCommand[2] + "\n";
        sendToAllMembers(currentChannel, msg);
    }
    catch(const std::exception& e)
    {
        return ;
    }
}

void    Client::sendMsgInChannel(Channel& RecieverChannel)
{
    std::vector<Client *>::iterator it = RecieverChannel.getMembers().begin();
    while (it != RecieverChannel.getMembers().end())
    {
        if ((*it)->getNickName() != _nickName)
        { 
            std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + " ";
            send((*it)->getFd(), msg.c_str(), msg.size(), 0);
            size_t i = 2;
            while (i < _splitedCommand.size())
            {
                send((*it)->getFd(), _splitedCommand[i].c_str() , _splitedCommand[i].size(), 0);
                if (i < _splitedCommand.size() - 1)
                    send((*it)->getFd(), " ", 1, 0);
                i++;
            }
            send((*it)->getFd(), "\n", 1, 0);
        }
        it++;
    }
}

void    Client::sendToAllMembers(Channel& currentChannel, std::string msg)
{
    std::vector<Client *>::iterator it = currentChannel.getMembers().begin();
    while (it != currentChannel.getMembers().end())
    {
        send((*it)->getFd(), msg.c_str(), msg.size(), 0);
        it++;
    }
}

bool    Client::ClientInChannel(Channel& channel)
{
    if (channel.getMemberByNick(_nickName) != NULL)
        return true;
    else
        return false;
}

bool    Client::isChannelOperator()
{
    try
    {
        Channel& currentChannel = _server.getChannelbyName(_splitedCommand[1]);
        std::vector<Client *>::iterator it = currentChannel.getOperators().begin();
        while (it != currentChannel.getOperators().end())
        {
            if ((*it)->getNickName() == _nickName) //you are operator
                return true;
            it++;
        }
    }
    catch (std::exception &e)
    {
        return false;
    }
    return false;
}

bool    Client::stringHasOnlyDigits()
{
    for (unsigned int i = 0; i < _splitedCommand[3].size(); i++)
    {
        if (!isdigit(_splitedCommand[3][i]))
            return false;
    }
    return true;
}

void    Client::needMoreParams()
{
    if (_splitedCommand[0] == "PASS" || _splitedCommand[0] == "USER" || _splitedCommand[0] == "KICK"
        || _splitedCommand[0] == "MODE" || _splitedCommand[0] == "INVITE" || _splitedCommand[0] == "TOPIC")
    {
        std::string msg = _splitedCommand[0] + ERR_NEEDMOREPARAMS;
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (_splitedCommand[0] == "NICK")
    {
        std::string msg = ERR_NONICKNAMEGIVEN;
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
}

bool    Client::isValidCommand()
{
    if (_splitedCommand[0] == "PASS" || _splitedCommand[0] == "NICK" || _splitedCommand[0] == "USER"
        || _splitedCommand[0] == "KICK" || _splitedCommand[0] == "MODE" || _splitedCommand[0] == "TOPIC"
        || _splitedCommand[0] == "PRIVMSG" || _splitedCommand[0] == "INVITE")
        return true;
    return false;
}

int    Client::moreLinesInBuffer()
{
    int newLine = 0;
    for (size_t i = 0; i < _buff.size(); i++)
    {
        if (_buff[i] == '\n')
            newLine++;
    }
    return newLine;
}

void    Client::splitByLine()
{
    std::istringstream iss(_buff);
    std::string line;

    while (std::getline(iss, line))
    {
        _splitMoreLines.push_back(line);        
    }
}
