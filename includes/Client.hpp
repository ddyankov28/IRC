/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:25:58 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/23 11:56:56 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Lib.hpp"
# include "Server.hpp"

class Server;

class Client
{
    private:
        Client();
        int                         _fd;
        std::string                 _userName;
        std::string                 _nickName;
        std::string                 _password;
        std::string                 _command;
        std::string                 _buff;
        bool                        _isRegistered;
        int                         _registerSteps;
        bool                        _passIsCorrect;
        std::vector<std::string>    _splitedCommand;
        Server&                     _server;
    public:
        Client(int cliFd, Server& server);
        ~Client();
    
    void    setFd(int pollFd);
    void    setUserName(std::string userName);
    void    setNickName(std::string& nickName);
    void    setCliCommand(std::string msg);
    void    setPassword(std::string pass);
    void    setBuff(std::string add);
    
    int         getFd();
    std::string getUserName();
    std::string getNickName();
    std::string getCliCommand();
    std::string getBuff();
    bool        getIsRegistered();
    
    void    checkFeatures();
    void    checkIfRegistered();
    void    checkCommand();
    void    splitCommand();
    void    joinChannels();
};

# endif
