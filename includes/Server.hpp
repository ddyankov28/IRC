/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:35:51 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/19 15:47:23 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
# define SERVER_HPP

# include "Lib.hpp"

class Server
{
    private:
        Server();
        int         _port;
        std::string _password;
        int         _sockFd;
    public:
        Server(char *av1, char *av2);
        ~Server();

        void        bindServ();

        // --- SETTERS --- //
        void        setSockFd();
        
        // --- GETTERS --- //
        int         getPort();
        std::string getPassword();
        int         getSockFd();
        
        
};

# endif
