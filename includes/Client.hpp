/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:25:58 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/08 14:24:40 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Lib.hpp"

class Client
{
    private:
        Client();
        std::string                 _userName;
        std::string                 _nickName;
        int                         _fd;
        std::string                 _command;
        bool                        _isRegistered;
        std::vector<std::string>    _splitedCommand;
        std::string                 _password;
        int                         _registerSteps;
        bool                        _passIsCorrect;
    public:
        Client(int cliFd);
        ~Client();
    
    void    setFd(int pollFd);
    void    setUserName(std::string userName);
    void    setNickName(std::string& nickName);
    void    setCliCommand(std::string msg);
    void    setPassword(std::string pass);
    void    checkCommand();
    void    splitCommand();
    void    checkIfRegistered();
    int         getFd();
    std::string getUserName();
    std::string getCliCommand();
    bool        getIsRegistered();
        
};

# endif
