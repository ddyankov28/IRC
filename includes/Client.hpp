/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vstockma <vstockma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:25:58 by ddyankov          #+#    #+#             */
/*   Updated: 2024/03/04 14:29:39 by vstockma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Lib.hpp"
# include "Server.hpp"

class Server;
class Channel;

class Client
{
    private:
        Client();
        int                         _fd;
        std::string                 _ip;
        std::string                 _userName;
        std::string                 _nickName;
        std::string                 _password;
        std::string                 _command;
        std::string                 _buff;
        bool                        _isRegistered;
        int                         _registerSteps;
        bool                        _passIsCorrect;
        std::vector<std::string>    _splitedCommand;
        std::vector<std::string>    _splitMoreLines;
        Server&                     _server;
    public:
        Client(int cliFd, Server& server);
        ~Client();
    
    void    setFd(int pollFd);
    void    setIp(std::string ip);
    void    setUserName(std::string userName);
    void    setNickName(std::string& nickName);
    void    setCliCommand(std::string msg);
    void    setPassword(std::string pass);
    void    setBuff(std::string add);
    
    int         getFd();
    std::string getIp();
    std::string getUserName();
    std::string getNickName();
    std::string getCliCommand();
    std::string getBuff();
    bool        getIsRegistered();
    std::vector<std::string> getSplitMoreLines();
    
    void    checkFeatures();
    void    checkIfRegistered();
    void    checkCommand();
    void    splitCommand();
    void    joinChannels();
    void    join(Channel& currentChannel);
    void    kickUsers();
    void    handleMode();
    bool    isChannelOperator();
    int     moreLinesInBuffer();
    void    splitByLine();
    bool    ClientInChannel(Channel& channel);
    void    sendMsgInChannel(Channel& RecieverChannel);
    void    needMoreParams();
    void    handleIandT();
    void    sendToAllMembers(Channel& currentChannel, std::string msg);
    void    handleFourParams();
    void    handleLimit();
    void    changeTopic();
    void    handleKeyChannel();
    bool    stringHasOnlyDigits();
    bool    isValidCommand();
    void    inviteUsers();

};

# endif
