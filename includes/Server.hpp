/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:35:51 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/01 10:47:19 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
# define SERVER_HPP

# include "Lib.hpp"
# include "Client.hpp"

class   Client;

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
        char                _buffer[256];
        std::vector<Client *> _clients;
    public:
        Server(char *av1, char *av2);
        ~Server();

        void        setAndRunServ();
        // Setup the server //
        void        bindServ();
        void        setSockFd();
        void        setAddr();
        void        listenServ();
        void        printFdStruct();
        
        void        acceptAndAddConnections();
        std::string       creationTime();
        void        setupErrorHandler(std::string msg);
        
};

# endif
