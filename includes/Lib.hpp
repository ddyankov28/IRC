/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lib.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:54:42 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/31 15:39:05 by ddyankov         ###   ########.fr       */
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

# define URED "\033[4;31m"
# define RED "\033[1;91m"
# define GREEN "\033[1;92m"
# define YELLOW "\033[1;93m"
# define CYAN "\033[1;96m"
# define RESET "\033[0m"
# define LINE  std::cout << "-------------------------" << std::endl;

# define MAX_CONNECTIONS 3
# define CREATION_TIME (_creationTime) ("The creation date of the server is " + _creationTime + "\r\n")

extern int shouldRun;

# endif
