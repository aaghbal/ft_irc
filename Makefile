# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/01 15:04:00 by aaghbal           #+#    #+#              #
#    Updated: 2024/03/27 22:10:08 by aaghbal          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
SRC = $(wildcard ./source/*.cpp)
INC = ./include/Channel.hpp ./include/Client.hpp  ./include/Error.hpp   ./include/Header.hpp  ./include/Server.hpp
NAME = ircserv
OBJ = $(SRC:.cpp=.o)
BONUS = $(wildcard ./bonus/*.cpp)
BONUS_OBJ = $(BONUS:.cpp=.o)
B_NAME = ircserv_bot
all: $(NAME)

bonus: ${B_NAME}

$(B_NAME):${BONUS_OBJ}
	${CC} ${CPPFLAGS} -I ./bonus/bot.hpp ${BONUS_OBJ} -o ${B_NAME}

$(NAME):${OBJ}
	${CC} ${CPPFLAGS} ${OBJ} -o ${NAME}

%.o: %.cpp $(INC) 
	${CC} ${CPPFLAGS} -c $< -o $@

clean :
	rm -rf ${OBJ} ${BONUS_OBJ}

fclean : clean 
	rm -rf ${NAME} ${B_NAME}

re : fclean all 

rebonus : 
	rm -rf ${B_NAME}
	make bonus