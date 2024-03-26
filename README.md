![](IRC.jpg)
## INTRODUCTION

Internet Relay Chat (IRC) is a text-based chat system for instant messaging. IRC is designed for group communication in discussion forums, called channels, but also allows one-on-one communication via private messages as well as chat and data transfer, including file sharing.

Internet Relay Chat is implemented as an application layer protocol to facilitate communication in the form of text. The chat process works on a client–server networking model. Users connect, using a client—which may be a web app, a standalone desktop program, or embedded into part of a larger program—to an IRC server, which may be part of a larger IRC network. Examples of programs used to connect include Mibbit, IRCCloud, KiwiIRC, and mIRC.

IRC usage has been declining steadily since 2003, losing 60 percent of its users. In April 2011, the top 100 IRC networks served more than 200,000 users at a time.

## IRC SERVER IN C++ STD=98

- executable will be run as follows:
    ./ircserv < port > < password >
- port: The port number on which the IRC Server will be listening to for incoming connections
- password: The connection password. It will be needed by the IRC client that tries to connect to the server
- The server is capable of handling multiple clients at the same time and never hang (use NetCat or Hexchat)
- All Input / Output operations are non-blocking
- Our reference client is Hexchat <a href="https://hexchat.github.io/"></a>

## SERVER FEATURES

- USER    -> authentication
- NICK    -> authentication
- PASSWORD-> authentication
- JOIN    -> joining channels
- PRIVMSG -> send message to a specific channel or to another client 
- KICK    -> Eject a client from the channel
- INVITE  -> Invite a client to a channel
- TOPIC   -> Change or view the channel topic
- MODE    -> Change the channel's mode
     -- i -> Set/ remove Invite-only channel
     -- t -> Set/ remove the restrictions of the TOPIC to channel operators
     -- k -> Set/ remove the channel key (password)
     -- o -> Give/ take channel operator privilege
     -- l -> Set/ remove the user limit to channel