# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ddyankov <ddyankov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/18 10:33:46 by ddyankov          #+#    #+#              #
#    Updated: 2024/02/01 13:45:32 by ddyankov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED_BACK = \033[0;101m
GREEN_BACK = \033[0;102m
COLOUR_END = \033[0m

NAME = ircserv

SRC =   srcs/main.cpp \
		srcs/Server.cpp srcs/SettingUpServer.cpp \
		srcs/Client.cpp

OBJ = $(patsubst srcs/%.cpp, obj/%.o, $(SRC))

DEPS = $(patsubst srcs/%.cpp, obj/%.d, $(SRC))

CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -MMD -MP -g -pedantic

.PHONY: all clean fclean re

obj/%.o: srcs/%.cpp
	@mkdir -p obj
	@c++ $(CPPFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	@c++ $(CPPFLAGS) $(OBJ) -o $@
	@echo "$(GREEN_BACK)✅✅✅Executable $(NAME) is ready✅✅✅$(COLOUR_END)"
	
clean:
		@rm -rf obj
		@echo "$(RED_BACK)🧹🧹🧹obj folder was cleaned🧹🧹🧹$(COLOUR_END)"

fclean: clean
		@rm -f $(NAME)
		@echo "$(RED_BACK)🧹🧹🧹Executable was cleaned🧹🧹🧹$(COLOUR_END)"

re: fclean all
-include $(DEPS)
