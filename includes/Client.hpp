/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vstockma <vstockma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:25:58 by ddyankov          #+#    #+#             */
/*   Updated: 2024/03/05 14:11:23 by vstockma         ###   ########.fr       */
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
        bool                        _passIsCorrect;
        int                         _registerSteps;
        std::vector<std::string>    _splitedCommand;
        std::vector<std::string>    _splitMoreLines;
        Server&                     _server;
    public:
		Client(int cliFd, Server& server);
		~Client();
    
		int							getFd() const;
		std::string					getIp() const;
		std::string					getUserName() const;
		std::string					getNickName() const;
		std::string					getCliCommand() const;
		std::string					getBuff() const;
		bool						getIsRegistered() const;
		std::vector<std::string>	getSplitMoreLines() const;

		void						setFd(int pollFd);
		void						setIp(std::string ip);
		void						setUserName(std::string userName);
		void						setNickName(std::string& nickName);
		void						setCliCommand(std::string msg);
		void						setPassword(std::string pass);
		void						setBuff(std::string add);

		void						splitCommand();
		int							moreLinesInBuffer();
		void						splitByLine();
		void						sendMsgInChannel(Channel& RecieverChannel);
		void						checkFeatures();

		void						privmsg();
		void						join(Channel& currentChannel);
		void						handleKeyChannel();
		void						joinChannels();
		void						kickUsers();
		void						inviteUsers();
		void						changeTopic();
		void						handleIandT();
		void						handleFourParams();
		void						handleMode();

		void						checkIfRegistered();
		void						checkCommand();
		bool						isChannelOperator();
		bool						ClientInChannel(Channel& channel);
		void						needMoreParams();
		void						sendToAllMembers(Channel& currentChannel, std::string msg);
		void						handleLimit();
		bool						stringHasOnlyDigits();
		bool						isValidCommand();

};

# endif
