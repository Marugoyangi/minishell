# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/05 14:27:11 by jeongbpa          #+#    #+#              #
#    Updated: 2023/12/05 06:36:41 by jeongbpa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#readline 없으면 설치하는 과정이 포함돼야함

SRC_DIR = ./srcs/
SRCS =	$(addprefix $(SRC_DIR), minishell.c \
								utils/libft.c utils/ascii_art.c utils/node_ctl.c \
								parse/tokenize.c parse/lexicize.c parse/parser.c \
								expand/expand.c expand/line_split.c expand/line_ctl.c expand/expand_tilde.c \
								expand/expand_asterisk.c expand/asterisk_filter.c \
								envp/envp_init.c envp/shell_var_init.c)
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