/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 14:31:02 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/22 16:30:03 by ddyankov         ###   ########.fr       */
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
        std::vector<Client *>         _operators;
        std::vector<Client *>         _members;
        std::string                 _topic;
        bool                        _isInviteChannel;
        bool                        _isTopicRestricted;
        std::string                 _channelKey;
        bool                        _isKeyChannel;
        int                         _limit;
        
    public:
        Channel(std::string name, Server& server);
        ~Channel();
        
        std::string                 getChannelName();
        bool                        getisInviteChannel();
        bool                        getisKeyChannel();
        std::vector<Client *>         getMembers();
        std::vector<Client *>         getOperators();

};

# endif
