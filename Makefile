# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/05 14:27:11 by jeongbpa          #+#    #+#              #
#    Updated: 2023/12/17 02:21:56 by woopinbell       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR = ./srcs/
BONUS_DIR = ./bonus_srcs/

SRCS =	$(addprefix $(SRC_DIR), minishell.c \
								utils/ascii_art.c utils/node_ctl.c utils/memory_ctl.c utils/general_utils.c \
								utils/expand_utils.c utils/line_ctl.c utils/line_split.c utils/sort_utils.c \
								parse/tokenize.c parse/lexicize.c parse/parser.c parse/syntax_check.c\
								parse/syntax_cmd.c parse/parsing_sort.c \
								expand/expand.c expand/expand_asterisk.c expand/asterisk_filter.c expand/expand_heredoc.c\
								expand/expand_split.c expand/asterisk_utils.c \
								initialize/envp_init.c \
								initialize/init.c initialize/signal.c\
								error/error.c \
								exec/exec_set_start.c \
								exec/exec_logical_operator.c exec/exec_pipeline.c exec/exec_subshell.c \
								exec/exec_redirection.c exec/exec_redirection_utils.c exec/exec_redirection_built_in.c \
								exec/exec_simple_command.c exec/exec_simple_command_utils.c \
								exec/exec_built_in.c exec/exec_built_in_env.c exec/exec_built_in_env_utils.c exec/exec_built_in_cd.c \
								exec/exec_heredoc.c exec/exec_heredoc_file.c)

# BONUS_SRCS = $(addprefix $(BONUS_DIR), minishell_bonus.c \
# 								utils/libft.c utils/ascii_art.c utils/node_ctl.c utils/memory_ctl.c utils/general_utils.c \
# 								utils/expand_utils.c utils/line_ctl.c utils/line_split.c\
# 								parse/tokenize.c parse/lexicize.c parse/parser.c parse/syntax_check.c\
# 								parse/syntax_cmd.c parse/parsing_sort.c\
# 								expand/expand.c expand/expand_asterisk.c expand/asterisk_filter.c expand/expand_heredoc.c\
# 								expand/expand_split.c \
# 								initialize/envp_init.c \
# 								initialize/init.c initialize/signal.c\
# 								error/error.c \
# 								exec/exec_libft.c \
# 								exec/exec_set_start.c \
# 								exec/exec_logical_operator.c exec/exec_pipeline.c exec/exec_subshell.c \
# 								exec/exec_redirection.c exec/exec_redirection_utils.c \
# 								exec/exec_simple_command.c exec/exec_simple_command_utils.c \
# 								exec/exec_built_in.c exec/exec_built_in_env.c exec/exec_built_in_env_utils.c exec/exec_built_in_cd.c \
# 								exec/exec_heredoc.c)

OBJS =	$(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRC:.c=.o)

HEADER = minishell.h
BONUS_HEADER = minishell_bonus.h
MAKEFILE = Makefile
NAME = minishell
LIBFT = ./srcs/libft/libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./srcs/ -gdwarf-4

ifeq ($(BONUS_FLAG), 42)
	OBJ_FLAG = $(BONUS_OBJS)
	OBJ_EXCEPT = $(OBJS)
	OBJ_HEADER = $(HEADER_BONUS)
else
	OBJ_FLAG = $(OBJS)
	OBJ_EXCEPT = $(BONUS_OBJS)
	OBJ_HEADER = $(HEADER)
endif

%.o: %.c $(OBJ_HEADER)
	 $(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME): $(OBJ_FLAG) $(LIBFT)
	@rm -f $(OBJ_EXCEPT)
	$(CC) $^ $(CFLAGS) -L./srcs/libft -lft -lreadline -lncurses -o $(NAME)

$(LIBFT):
	@make -C ./srcs/libft

re: fclean all

clean:
	rm -f $(OBJS) $(BONUS_OBJS)
	@$(MAKE) -C ./srcs/libft fclean

fclean: clean
	rm -f $(NAME)

.PHONY: all clean fclean re bonus