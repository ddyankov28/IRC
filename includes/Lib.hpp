/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lib.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:54:42 by ddyankov          #+#    #+#             */
/*   Updated: 2024/03/06 11:39:06 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef LIB_HPP
# define LIB_HPP

# include <iostream>
# include <cstdlib>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <cstring>
# include <unistd.h>
# include <cstdio>
# include <poll.h>
# include <vector>
# include <fcntl.h>
# include <csignal>
# include <ctime>
# include <sstream>
# include <algorithm>
# include <arpa/inet.h>

# define URED "\033[4;31m"
# define RED "\033[1;91m"
# define GREEN "\033[1;92m"
# define YELLOW "\033[1;93m"
# define CYAN "\033[1;96m"
# define RESET "\033[0m"
# define LINE  std::cout << "-------------------------" << std::endl;

# define MAX_CONNECTIONS 10
# define REGISTER "You are connected but not registered yet\n"
# define ERR_NOTEXTTOSEND ":No text to send\n"
# define ERR_NOSUCHNICK " :No such nick/channel\n"
# define ERR_NORECIPIENT ":42_IRC 411 "
# define ERR_NORECIP " :No recipient given "
# define ERR_NOTONCHANNEL " :You're not on that channel\n"
# define ERR_CHANNELISFULL " :Cannot join channel (+l)\n"
# define ERR_BADCHANNELKEY " :Cannot join channel (+k)\n"
# define ERR_NEEDMOREPARAMS " :Not enough parameters\n"
# define ERR_NONICKNAMEGIVEN ":No nickname given\n"
# define ERR_UNKNOWNMODE " :is unknown mode to me\n"
# define ERR_INVITEONLYCHAN " :Cannot join channel (+i)\n"

extern int shouldRun;

# endif
