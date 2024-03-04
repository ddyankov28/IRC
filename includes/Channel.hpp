/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vstockma <vstockma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 14:31:02 by ddyankov          #+#    #+#             */
/*   Updated: 2024/03/04 14:12:15 by vstockma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Lib.hpp"
# include "Server.hpp"
# include "Client.hpp"

class Server;
class Client;

class Channel
{
    private:
        std::string                 _name;
        Server&                     _server;
        std::vector<Client *>       _operators;
        std::vector<Client *>       _members;
        std::vector<std::string>    _invitedUsers;
        std::string                 _topic;
        bool                        _isInviteChannel;
        bool                        _isTopicRestricted;
        std::string                 _channelKey;
        bool                        _isKeyChannel;
        int                         _limit;
        
    public:
        Channel(std::string name, Server& server);
        ~Channel();
        Channel&  operator=(const Channel& inst);
        
        std::string                 getChannelName();
        bool                        getisInviteChannel();
        bool                        getisKeyChannel();
        int                         getLimit();
        std::string                 getchannelKey();
        std::vector<Client *>&      getMembers();
        std::vector<Client *>&      getOperators();
        std::vector<std::string>    getinvitedUsers();

        Client*                     getMemberByNick(std::string Nick);
        Client*                     getOpByNick(std::string Nick);

        void                        setisInviteChannel(char c);
        void                        setisTopicRestricted(char c);
        void                        setlimit(char c, std::string limit);
        void                        setKeyChannel(char c, std::string key);
        void                        setOperator(char c, std::string Nickname);

        int                         UserIsInvited(std::string Nick, int sw);

};

# endif
