/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:35:51 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/28 15:53:23 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
# define SERVER_HPP

# include "Lib.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class   Client;
class   Channel;

class   Server
{
    private:
        Server();
        std::string         _creationTime;
        int                 _port;
        std::string         _password;
        int                 _serverFd;
        struct sockaddr_in  _servAddr;
        pollfd              _polls[MAX_CONNECTIONS];
        int                 _fdsCounter;
        std::vector<Client *> _clients;
        std::vector<Channel> _channels;
    public:
        Server(char *av1, char *av2);
        ~Server();

        // main loop //
        void        setAndRunServ();
        void        acceptAndAddConnections();
        void        handleEvents();
        void        itsClient();
        // Setup the server //
        void        bindServ();
        void        setSockFd();
        void        setAddr();
        void        listenServ();
        void        printFdStruct();
        void        setupErrorHandler(std::string msg);
        
        std::string creationTime();
        Client*     getClient(int fd);
        Client*     getClientByNick(std::string Nick);
        Client*     getClientByUser(std::string User);
        Channel&    getChannelbyName(std::string Channel);
        std::vector<Channel>&   getChannels();
        void        itsClient(int i);
        void        removeClient(int fd);
        void        deleteChannel(std::string channelName);

};

# endif
