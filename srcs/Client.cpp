/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:04:57 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/28 16:56:47 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Client.hpp"

Client::Client(int cliFd, Server& server) : _fd(cliFd), _userName(""), _nickName(""), _command(""), _buff(""), _isRegistered(false), _registerSteps(0), _passIsCorrect(false), _server(server)
{}

Client::~Client()
{}

void    Client::setFd(int pollFd)
{
    _fd = pollFd;
}

void    Client::setIp(std::string ip)
{
    _ip = ip;
}

void    Client::setUserName(std::string userName)
{
    _userName = userName;
}

void    Client::setNickName(std::string& nickName)
{
    _nickName = nickName;
}

void    Client::setCliCommand(std::string msg)
{
    _command = msg;
}

int Client::getFd()
{
    return _fd;
}

std::string Client::getIp()
{
    return _ip;
}

std::string Client::getUserName()
{
    return _userName;
}

std::string Client::getNickName()
{
    return _nickName;
}

std::string Client::getCliCommand()
{
    return _command;
}

bool    Client::getIsRegistered()
{
    return _isRegistered;
}

void    Client::setBuff(std::string add)
{
    _buff = add;
}

std::string Client::getBuff()
{
    return _buff;    
}

std::vector<std::string> Client::getSplitMoreLines()
{
    return _splitMoreLines;
}

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

void    Client::setPassword(std::string pass)
{
    _password = pass;
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

void    Client::checkFeatures()
{
    if (_splitedCommand.size() < 1)
        return;
    if (_splitedCommand[0] == "PRIVMSG" && _splitedCommand.size() == 1)
    {
        std::string msg = ERR_NORECIPIENT + _splitedCommand[0] + "\n";
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (_splitedCommand[0] == "PRIVMSG")
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
    else if (_splitedCommand[0] == "JOIN")
            joinChannels();
    else if (_splitedCommand[0] == "KICK")
        kickUsers();
    else if (_splitedCommand[0] == "MODE" && _splitedCommand.size() >= 3)
    {
        std::cout << "Here" << std::endl;
        if (isChannelOperator())
            handleMode();
        else
            std::cout << "No operator " << std::endl;
    }
    _splitedCommand.erase(_splitedCommand.begin(), _splitedCommand.end());
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


void    Client::handleMode()
{
    std::cout << "Is operator " << std::endl;
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
                                    {
                                        std::cout << _server.getChannels()[0].getChannelName() << std::endl;
                                        _server.getChannels().erase(it);
                                    }
                                        //std::cout << it->getChannelName() << std::endl;
                                    it++;
                                }
                            }
                            return ;
                        }
                        else
                            ++itMembers;   
                    }
                    send(_fd, _splitedCommand[2].c_str(), _splitedCommand[2].size(), 0);
                    send(_fd, " :No such Nick on channel\n", 26, 0);
                    return ;
                }
                it++;
            }
            send(_fd, _splitedCommand[1].c_str(), _splitedCommand[1].size(), 0);
            send(_fd, " :You're not channel operator\n", 30, 0);  
        }
        else
        {
            std::string msg = _splitedCommand[1] + ERR_NOTONCHANNEL;
            send(_fd, msg.c_str(), msg.size(), 0);
        }  
    }
    catch (std::exception& e)
    {
        send(_fd, _splitedCommand[1].c_str(), _splitedCommand[1].size(), 0);
        send(_fd, " :No such channel\n", 18, 0);
    }
}

void    Client::joinChannels()
{
    if (_splitedCommand[1][0] == '#')
        {
            try
            {
                Channel& currentChannel =_server.getChannelbyName(_splitedCommand[1]);
                // if (newChannel.getisInviteChannel())
                // {
                    
                // }
                if (currentChannel.getisKeyChannel() && _splitedCommand.size() > 2)
                {
                    
                }  
                else if (!currentChannel.getisKeyChannel())
                {
                    if (currentChannel.getMemberByNick(_nickName))
                    {
                        send(_fd, ":Nick is already in channel\n", 28, 0);
                        return ;
                    }
                    currentChannel.getMembers().push_back(this);

                    std::vector<Client *>::iterator it = currentChannel.getMembers().begin();
                    while (it != currentChannel.getMembers().end())
                    {
                        std::string msg = ":" + getNickName() + "!" + getUserName() + "@" + _ip + " " + _splitedCommand[0] + " " + _splitedCommand[1] + "\n";
                        send((*it)->getFd(), msg.c_str(), msg.size(), 0);
                        it++;
                    }
                    std::string msg = ":42_IRC " + getNickName() + " = "  + _splitedCommand[1] +  " :";
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
                msg = ":42_IRC " + getNickName() + " = "  + _splitedCommand[1] + " :@" + getNickName() + "\n";
                send(getFd(), msg.c_str(), msg.size(), 0);
            }          
        }
}

void    Client::needMoreParams()
{
    if (_splitedCommand[0] == "PASS")
    {
        std::string msg = _splitedCommand[0] + ERR_NEEDMOREPARAMS;
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (_splitedCommand[0] == "NICK")
    {
        std::string msg = ERR_NONICKNAMEGIVEN;
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (_splitedCommand[0] == "USER")
    {
        std::string msg = _splitedCommand[0] + ERR_NEEDMOREPARAMS;
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
    else if (_splitedCommand[0] == "KICK")
    {
        std::string msg = _splitedCommand[0] + ERR_NEEDMOREPARAMS;
        send(getFd(), msg.c_str(), msg.size(), 0);
    }
}


void    Client::checkCommand()
{
    //std::cout << _password << std::endl;
    if (_splitedCommand.size() < 1)
        return;
    if (_splitedCommand.size() == 1)
    {
        needMoreParams();
        return ;
    }
    if (((_splitedCommand[0] == "PASS" && _passIsCorrect == false) || _splitedCommand[0] == "NICK" || _splitedCommand[0] == "USER") && _splitedCommand.size() >= 2)
    {
        if (_splitedCommand[0] == "PASS" && _splitedCommand[1] != _password)
        {
            std::cout << _fd << std::endl;
            send(_fd, "⛔️Password is incorrect⛔️\n", 34, 0);
        }
        else if (_splitedCommand[0] == "PASS" && _splitedCommand[1] == _password)
        {
            _registerSteps++;
            _passIsCorrect = true;
        }
        else if (_splitedCommand[0] == "NICK")
        {
            if (_server.getClientByNick(_splitedCommand[1]))
            {
                send(_fd, "⛔️Nickname already in use⛔️\n", 36, 0);
                return ;
            }
            setNickName(_splitedCommand[1]);
            _registerSteps++;
        }
        else if (_splitedCommand[0] == "USER")
        {
            if (_server.getClientByUser(_splitedCommand[1]))
            {
                send(_fd, "Username already in use\n", 24, 0);
                return ;
            }
            setUserName(_splitedCommand[1]);
            _registerSteps++;
        }
    }
    else if (_splitedCommand[0] == "PASS" && _passIsCorrect == true)
        send(_fd, "✅You already provided a correct password✅\n", 46, 0);
    else if (_isRegistered == false)
    {
        if (_splitedCommand[0] != "CAP" && _splitedCommand[1] != "LS")
            send(_fd, "🚫Wrong command, you have to register yourself🚫\n", 53, 0);
    }
    if (_registerSteps == 3)
    {
        _isRegistered = true;
        _registerSteps++;
        send(_fd, "✅You are already registered✅\n", 33, 0);
    }
    //std::cout << "NICKNAME FOR USER WITH FD " << _fd << " IS: " << _nickName;
    //std::cout << "USERNAME FOR USER WITH FD " << _fd << " IS: " << _userName << std::endl;
}

