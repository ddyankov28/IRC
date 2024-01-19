/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:40:17 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/19 14:37:13 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

int main(int ac, char **av)
{
    (void)av;
    if (ac != 3)
    {
        std::cerr << RED << "Wrong number of arguments" << std::endl;
        std::cerr << "Correct Usage ./ircserv <port> <password>" << RESET <<std::endl;
        return 1;
    }
    std::cout << "IRC SERVER" << std::endl;
    // std::cout << "PORT: " << av[1] << "\nPASSWORT: " << av[2] << std::endl;
    Server  irc(av[1], av[2]);
    
    //std::cout << "PORT: " << irc.getPort() << "\nPASSWORT: " << irc.getPassword() << std::endl;

    
    return 0;
}
