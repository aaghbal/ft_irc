# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaghbal <aaghbal@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/01 15:04:00 by aaghbal           #+#    #+#              #
#    Updated: 2024/03/10 12:06:40 by aaghbal          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
SRC = $(wildcard *.cpp)
INC = $(wildcard *.hpp)
NAME = ircserv
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME):${OBJ}
	${CC} ${CPPFLAGS} ${OBJ} -o ${NAME}

%.o: %.cpp $(INC)
	${CC} ${CPPFLAGS} -c $< -o $@

clean :
	rm -rf ${OBJ}

fclean : clean
	rm -rf ${NAME}

re : fclean all