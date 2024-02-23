/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:04:57 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/23 13:33:12 by ddyankov         ###   ########.fr       */
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

void    Client::setPassword(std::string pass)
{
    _password = pass;
}

void    Client::checkFeatures()
{
    if ((_splitedCommand[0] == "PRIVMSG"  && _splitedCommand.size() >= 3) || (_splitedCommand[0] == "JOIN" && _splitedCommand.size() >= 2) || (_splitedCommand[0] == "QUIT"))
    {
        if (_splitedCommand[0] == "PRIVMSG")
        {
            Client* Reciever = _server.getClientByNick(_splitedCommand[1]);
            if (!Reciever)
            {
                std::string msg = "@localhost: ";
                send(getFd(), msg.c_str(), msg.size(), 0);
                send(_fd, "⛔️No such Nick⛔️\n", 26, 0);
            }
            else
            {
                std::string msg = getUserName() + "!" + getNickName() + "@localhost: ";
                send(Reciever->getFd(), msg.c_str(), msg.size(), 0);
                size_t i = 2;
                while (i <= _splitedCommand.size())
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
    }
    _splitedCommand.clear();
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
                        send(_fd, "⛔️Nick already in channel⛔️\n", 37, 0);
                        return ;
                    }
                    currentChannel.getMembers().push_back(this);
                } 
            }
            catch (std::exception& e)
            {
                Channel newChannel(_splitedCommand[1], _server);

                newChannel.getMembers().push_back(this);
                newChannel.getOperators().push_back(this);
                _server.getChannels().push_back(newChannel);
            }          
        }
}


void    Client::checkCommand()
{
    //std::cout << _password << std::endl;
    if (((_splitedCommand[0] == "PASS" && _passIsCorrect == false) || _splitedCommand[0] == "NICK" || _splitedCommand[0] == "USER") && _splitedCommand.size() == 2)
    {
        if (_splitedCommand[0] == "PASS" && _splitedCommand[1] != _password)
            send(_fd, "⛔️Password is incorrect⛔️\n", 35, 0);
        else if (_splitedCommand[0] == "PASS" && _splitedCommand[1] == _password)
        {
            _registerSteps++;
            _passIsCorrect = true;
        }
        else if (_splitedCommand[0] == "NICK")
        {
            if (_server.getClientByNick(_splitedCommand[1]))
            {
                send(_fd, "⛔️Nickname already in use⛔️\n", 37, 0);
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
        send(_fd, "✅You already provided a correct password✅\n", 47, 0);
    else if (_isRegistered == false)
        send(_fd, "🚫Wrong command, you have to register yourself🚫\n", 54, 0);
    if (_registerSteps == 3)
    {
        _isRegistered = true;
        _registerSteps++;
        send(_fd, "✅You are already registered✅\n", 34, 0);
    }
    //std::cout << "NICKNAME FOR USER WITH FD " << _fd << " IS: " << _nickName;
    //std::cout << "USERNAME FOR USER WITH FD " << _fd << " IS: " << _userName << std::endl;
}

