/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:35:51 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/23 12:46:22 by ddyankov         ###   ########.fr       */
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
        int                 _sockFd;
        struct sockaddr_in  _servAddr;
    public:
        Server(char *av1, char *av2);
        ~Server();

        void        setServ();
        // Setup the server //
        void        bindServ();
        void        setSockFd();
        void        setAddr();
        void        listenServ();
        
        
};

# endif
