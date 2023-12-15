# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/05 14:27:11 by jeongbpa          #+#    #+#              #
#    Updated: 2023/12/15 15:30:37 by woopinbell       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#readline 없으면 설치하는 과정이 포함돼야함

SRC_DIR = ./srcs/
SRCS =	$(addprefix $(SRC_DIR), minishell.c \
								utils/libft.c utils/ascii_art.c utils/node_ctl.c utils/memory_ctl.c \
								parse/tokenize.c parse/lexicize.c parse/parser.c parse/syntax_check.c\
								parse/syntax_subshell.c parse/parsing_sort.c\
								expand/expand.c expand/line_split.c expand/line_ctl.c expand/expand_tilde.c \
								expand/expand_asterisk.c expand/asterisk_filter.c \
								initialize/envp_init.c initialize/shell_var_init.c \
								initialize/init.c \
								error/error.c \
								exec/exec_libft.c \
								exec/exec_set_start.c \
								exec/exec_logical_operator.c exec/exec_pipeline.c exec/exec_subshell.c \
								exec/exec_redirection.c exec/exec_redirection_utils.c \
								exec/exec_simple_command.c exec/exec_simple_command_utils.c \
								exec/exec_built_in.c exec/exec_built_in_env.c exec/exec_built_in_env_utils.c \
								exec/exec_heredoc.c)

# exec/exe_set_start.c exec/exec_simple_command.c exec/exec_libft.c \
								# exec/exec_built_in.c exec/exec_built_in_env.c exec/exec_built_in_env_utils.c \
								# exec/exec_logical_subshell_pipe.c exec/exec_redirection.c \								
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