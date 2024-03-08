/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:40:17 by ddyankov          #+#    #+#             */
/*   Updated: 2024/03/07 09:10:29 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

int shouldRun = 1;

void    sigIntHandler(int sig)
{
    (void)sig;
    shouldRun = 0;
    std::cout << RED << "\nExiting the server because you pressed CTRL + C" << RESET << std::endl;
}

void    checkArgs(int ac)
{
    if (ac != 3)
        throw std::out_of_range("Wrong number of arguments\nCorrect Usage ./ircserv <port> <password>");
}

void    checkPort(char *port)
{
    if (atoi(port) < 6665 || atoi(port) > 6669)
        throw std::out_of_range("Port is out of range use a port between 6665 - 6669");
}

int main(int ac, char **av)
{
    try
    {
        signal(SIGINT, sigIntHandler);
        checkArgs(ac);
        checkPort(av[1]);
        
        Server  irc(av[1], av[2]);
        
        irc.setAndRunServ();
    }
    catch(std::exception& e)
    {
        std::cerr << RED << e.what() << RESET << std::endl;
    }
    return 0;
}
