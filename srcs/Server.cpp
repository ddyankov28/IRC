/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:57:00 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/22 14:26:51 by ddyankov         ###   ########.fr       */
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
    _polls[_fdsCounter].fd = newFd;
    _polls[_fdsCounter].events = POLLIN;
    _polls[_fdsCounter].revents = 0;
    send(_polls[_fdsCounter].fd, "👋--- Welcome to the 42_IRC Server ---👋\nCreated on ", 57, 0);
    send(_polls[_fdsCounter].fd, _creationTime.c_str(), _creationTime.size(), 0);
    send(_polls[_fdsCounter].fd, REGISTER, sizeof(REGISTER), 0);
    std::cout << "Server accepted a connection" << std::endl;
    Client* newClient = new Client(_polls[_fdsCounter].fd, *this);
    _clients.push_back(newClient);
    std::cout << "Client was added" << std::endl;
    _fdsCounter++;
}

void    Server::itsClient(int i)
{
    char buffer[512];
    std::string buff;
    Client* currentCli = getClient(_polls[i].fd);
    
    if (!currentCli)
        throw std::runtime_error("Could not find Client");
    int bytes = recv(_polls[i].fd, buffer, sizeof(buffer), 0);
    // Connection closed or Error
    if (bytes <= 0)
    {
        if (!bytes)
            std::cout << RED << "Connection " << _polls[i].fd << " was closed" << RESET << std::endl;
        else
            perror("Error");
        close(_polls[i].fd);
        _fdsCounter--;
        std::vector<Client *>::iterator it = _clients.begin();
        int cl = 0;
        while (it != _clients.end())
        {
            if ((*it)->getFd() == currentCli->getFd())
            {
                close(currentCli->getFd());
                delete _clients[cl];
                _clients.erase(it);
                break ;
            }
            it++;
            cl++;
        }
    }
    else
    {
        buffer[bytes] = '\0';
        buff = buffer;
        if (buff.find_first_of("\r\n") == std::string::npos)
        {
            // std::cout << "THERE WAS CTRL + D PRESSED " << std::endl;
            currentCli->setBuff(currentCli->getBuff() + buff);
            std::cout << "After ctrl D "<< currentCli->getBuff() << std::endl;
        }
        else
        {
            currentCli->setBuff(currentCli->getBuff() + buff);
            // std::cout << "THERE WAS ENTER PRESSED" << std::endl;
            currentCli->setCliCommand(currentCli->getBuff());
            std::cout << "---[ Message from Client ] ---" << std::endl << currentCli->getBuff() << std::endl;
            currentCli->setPassword(_password);
            currentCli->splitCommand();
            currentCli->checkCommand();
            currentCli->checkFeatures();
            currentCli->setBuff("");
        }
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
    return NULL;
}
