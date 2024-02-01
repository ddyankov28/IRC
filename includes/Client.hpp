/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:25:58 by ddyankov          #+#    #+#             */
/*   Updated: 2024/02/01 10:07:56 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Lib.hpp"

class Client
{
    private:
        std::string _userName;
        std::string _nickName;
        int         _clientFd;
    public:
        Client();
        ~Client();
    
    void    setFd(int pollFd);
    void    setUserName(std::string userName);

    int         getFd();
    std::string getUserName();
        
};

# endif
