# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/05 14:27:11 by jeongbpa          #+#    #+#              #
#    Updated: 2023/11/30 07:49:09 by jeongbpa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#readline 없으면 설치하는 과정이 포함돼야함

SRC_DIR = ./srcs/
SRCS =	$(addprefix $(SRC_DIR), minishell.c libft.c ascii_art.c parse/tokenize.c parse/node_ctl.c parse/lexicize.c \
								parse/parser.c)
OBJS =	$(SRCS:.c=.o)

HEADER = minishell.h
MAKEFILE = Makefile
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./srcs/ -g
%.o: %.c $(HEADER)
	 $(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME): $(OBJS)
	$(CC) $^ $(CFLAGS) -lreadline -o $(NAME)

all: $(NAME)

clean: 
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re