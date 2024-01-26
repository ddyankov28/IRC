/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:35:51 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/26 13:50:23 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
# define SERVER_HPP

# include "Lib.hpp"

class Server
{
    private:
        Server();
        int                 _port;
        std::string         _password;
        int                 _servSockFd;
        struct sockaddr_in  _servAddr;
        int                 _newSockFd;
        pollfd              _polls[MAX_CONNECTIONS];
        int                 _usersCounter;
        bool                _shouldRun;
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
        
        void        acceptConnections();
        
};

# endif
