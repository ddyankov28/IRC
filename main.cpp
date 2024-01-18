/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:40:17 by ddyankov          #+#    #+#             */
/*   Updated: 2024/01/18 13:04:16 by ddyankov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"

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
    int sock_fd = socket()
    return 0;
}
