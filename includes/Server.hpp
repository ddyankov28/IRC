/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:35:51 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/19 14:37:22 by ddyankov         ###   ########.fr       */
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

        // --- GETTERS --- //
        int         getPort();
        std::string getPassword();
        
};

# endif
