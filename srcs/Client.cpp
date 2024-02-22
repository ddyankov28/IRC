/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:04:57 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/22 10:21:35 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Client.hpp"

Client::Client(int cliFd) : _fd(cliFd), _userName(""), _nickName(""), _command(""), _buff(""), _isRegistered(false), _registerSteps(0), _passIsCorrect(false)
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
    }*/
}

void    Client::setPassword(std::string pass)
{
    _password = pass;
}


void    Client::checkCommand()
{
    //std::cout << _password << std::endl;
    if (((_splitedCommand[0] == "PASS" && _passIsCorrect == false) || _splitedCommand[0] == "NICK" || _splitedCommand[0] == "USER") && _splitedCommand.size() == 2)
    {
        if (_splitedCommand[0] == "PASS" && _splitedCommand[1] != _password)
            send(_fd, "Password is incorrect!\n", 24, 0);
        else if (_splitedCommand[0] == "PASS" && _splitedCommand[1] == _password)
        {
            _registerSteps++;
            _passIsCorrect = true;
        }
        else if (_splitedCommand[0] == "NICK")
        {
            setNickName(_splitedCommand[1]);
            _registerSteps++;
        }
        else if (_splitedCommand[0] == "USER")
        {
            setUserName(_splitedCommand[1]);
            _registerSteps++;
        }
    }
    else if (_splitedCommand[0] == "PASS" && _passIsCorrect == true)
        send(_fd, "✅You already provided a correct password✅\n", 47, 0);
    else if (_isRegistered == false)
        send(_fd, "🚫Wrong command, you have to register yourself🚫\n", 54, 0);
    _splitedCommand.clear();
    if (_registerSteps == 3)
    {
        _isRegistered = true;
        _registerSteps++;
        send(_fd, "✅You are already registered✅\n", 34, 0);
    }
    //std::cout << "NICKNAME FOR USER WITH FD " << _fd << " IS: " << _nickName;
    //std::cout << "USERNAME FOR USER WITH FD " << _fd << " IS: " << _userName << std::endl;
}
