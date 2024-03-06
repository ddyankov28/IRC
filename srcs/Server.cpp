/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/03/06 17:00:51 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

// Constructor
Server::Server(char *av1, char *av2) : _port(atoi(av1)), _password(av2), _fdsCounter(0)
{
    _creationTime = creationTime();
}

// Destructor
Server::~Server()
{
    for (unsigned int i = 0; i < _clients.size(); i++)
        delete _clients[i];
}

void    Server::acceptAndAddConnections()
{
    int newFd = 0;
    
    struct sockaddr_in  newConnection;
    socklen_t newConnectionLen = sizeof(newConnection);
    if ((newFd = accept(_serverFd, (struct sockaddr *) &newConnection, &newConnectionLen)) == -1)
        setupErrorHandler("Accepting a new connection Error");
    char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &newConnection, str, INET_ADDRSTRLEN);
    _polls[_fdsCounter].fd = newFd;
    _polls[_fdsCounter].events = POLLIN;
    _polls[_fdsCounter].revents = 0;
    send(_polls[_fdsCounter].fd, "👋--- Welcome to the 42_IRC Server ---👋\nCreated on ", 56, 0);
    send(_polls[_fdsCounter].fd, _creationTime.c_str(), _creationTime.size(), 0);
    send(_polls[_fdsCounter].fd, "You are connected but not registered yet\n", 41, 0);
    std::cout << "Server accepted a connection" << std::endl;
    Client* newClient = new Client(_polls[_fdsCounter].fd, *this);
    newClient->setIp(str);
    _clients.push_back(newClient);
    std::cout << "Client was added" << std::endl;
    _fdsCounter++;
}

void    Server::itsClient(int i)
{
    char buffer[513];
    std::string buff;
    Client* currentCli = getClient(_polls[i].fd);
    
    if (!currentCli)
        throw std::runtime_error("Could not find Client");
    int bytes = recv(_polls[i].fd, buffer, sizeof(buffer), 0);
    // Connection closed or Error
    if (bytes > 512)
        send(_polls[i].fd, "Message limit is 512 characters\n", 32, 0);
    else if (bytes <= 0)
    {
        if (!bytes)
        {
            removeClient(_polls[i].fd);
            std::cout << RED << "Connection " << _polls[i].fd << " was closed" << RESET << std::endl;
        }
        else
            perror("Error");
        removeClient(_polls[i].fd);
        std::cout << RED << "Connection " << _polls[i].fd << " was closed" << RESET << std::endl;
        close(_polls[i].fd);
        _fdsCounter--;
        std::cout << RED << _clients.size() << " users left in the server" << RESET << std::endl;
    }
    else
    {
        buffer[bytes] = '\0';
        buff = buffer;
        if (buff.find_first_of("\r\n") == std::string::npos)
        {
            // std::cout << "THERE WAS CTRL + D PRESSED " << std::endl;
            currentCli->setBuff(currentCli->getBuff() + buff);
        }
        else
        {
            currentCli->setPassword(_password);
            currentCli->setBuff(currentCli->getBuff() + buff);
            // std::cout << "THERE WAS ENTER PRESSED" << std::endl;
            currentCli->setCliCommand(currentCli->getBuff());
            std::cout << "---[ Message from Client ] ---" << std::endl << currentCli->getBuff() << std::endl;
            
            if (currentCli->moreLinesInBuffer() >= 2)
            {
                size_t i = 0;
                size_t lines = currentCli->moreLinesInBuffer();
                while (i < lines)
                {
                    if (i == 0)
                        currentCli->splitByLine();
                    currentCli->setCliCommand(currentCli->getSplitMoreLines()[i]);
                    currentCli->splitCommand();
                    currentCli->checkCommand();
                    currentCli->checkFeatures();
                    currentCli->setBuff("");
                    i++;
                }
            }
            else
            {
                currentCli->splitCommand();
                currentCli->checkCommand();
                currentCli->checkFeatures();
                currentCli->setBuff("");
            }
            
        }
    }
}

void    Server::removeClient(int fd)
{
    std::vector<Client *>::iterator itClients = _clients.begin();
    while (itClients != _clients.end())
    {
        if ((*itClients)->getFd() == fd)
        {
            for (size_t i = 0; i < _channels.size(); i++)
            {
                std::vector<Client *>::iterator itOperators = _channels[i].getOperators().begin();
                while (itOperators != _channels[i].getOperators().end())
                {
                    if ((*itOperators)->getNickName() == (*itClients)->getNickName())
                        _channels[i].getOperators().erase(itOperators);
                    else
                        ++itOperators;
                }
                std::vector<Client *>::iterator itMembers = _channels[i].getMembers().begin();
                while (itMembers != _channels[i].getMembers().end())
                {
                    if ((*itMembers)->getNickName() == (*itClients)->getNickName())
                    {
                        std::cout << RED << "User " << (*itClients)->getNickName() << " was removed from the channel" << RESET << std::endl;
                        _channels[i].getMembers().erase(itMembers);
                    }
                    else
                        ++itMembers;
                }      
            }
            delete *itClients;
            _clients.erase(itClients);
        }
        else
           ++itClients;
    }
}

void    Server::handleEvents()
{
    // Check existing connections looking for data ro read //
    for (int i = 0; i < _fdsCounter; i++)
    {
        // Check if someone is ready to read //
        if (_polls[i].revents & POLLIN)
        {
            // If it is the server, we handle the new connection //
            if (_polls[i].fd == _serverFd)
            {
                std::cout << GREEN << "Server ready to accept" << RESET << std::endl;
                if (_fdsCounter + 1 > MAX_CONNECTIONS)
                    throw std::out_of_range("Max Connections limit is reached");
                acceptAndAddConnections();
            }
            else // It is client //
                itsClient(i);
        }
    }
}

void    Server::setAndRunServ()
{
    setSockFd();
    setAddr();
    bindServ();
    listenServ();
    std::cout << GREEN << "🏃---Server is running---🏃" << RESET << std::endl;
    while (shouldRun)   // The main server loop
    {
        int numEvents = poll(_polls, _fdsCounter, -1);
        if (numEvents < 0)
        {
            std::vector<Client *>::iterator it = _clients.begin();
            while (it != _clients.end())
            {
                send((*it)->getFd(), "QUIT :Client", 13, 0);
                close((*it)->getFd());
                it++;
            }   
            setupErrorHandler("Poll Error");
        }
        else if (numEvents > 0)
            handleEvents();
    }
}

void    Server::printFdStruct()
{
    for (int i = 0; i < _fdsCounter; i++)
    {
        std::cout << YELLOW << i << " Connection events: " << _polls[i].events << RESET << std::endl;
        std::cout << YELLOW << i << " Connection fd: " << _polls[i].fd << RESET << std::endl;
        std::cout << YELLOW << i << " Connection revents: " << _polls[i].revents << RESET << std::endl;
        LINE;
    }
}

std::string Server::creationTime()
{
    time_t      rawtime;
    struct tm*  timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}

Client* Server::getClient(int fd)
{
    std::vector<Client *>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if ((*it)->getFd() == fd)
            return *it;
        it++;
    }   
    return NULL;
}

Client* Server::getClientByNick(std::string Nick)
{
    std::vector<Client *>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if ((*it)->getNickName() == Nick)
            return *it;
        it++;
    }
    try
    {
        getChannelbyName(Nick);
    }
    catch(const std::exception& e)
    {
        return NULL;
    }
    return NULL;
}

Client* Server::getClientByUser(std::string User)
{
    std::vector<Client *>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if ((*it)->getUserName() == User)
            return *it;
        it++;
    }   
    return NULL;
}

Channel& Server::getChannelbyName(std::string channelName)
{
    std::vector<Channel>::iterator it = _channels.begin();
    while (it != _channels.end())
    {
        if (it->getChannelName() == channelName)
            return *it;
        it++;
    }
    throw std::out_of_range("Channel not found");
}

std::vector<Channel>&    Server::getChannels()
{
    return _channels;
}
