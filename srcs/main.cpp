/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:40:17 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/24 15:32:32 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << RED << "Wrong number of arguments" << std::endl;
        std::cerr << "Correct Usage ./ircserv <port> <password>" << RESET <<std::endl;
        return 1;
    }
    try
    {
        Server  irc(av[1], av[2]);

        irc.setAndRunServ();
    }
    catch(std::exception& e)
    {
        std::cerr << RED << e.what() << RESET << std::endl;
    }
    
    
    return 0;
}
