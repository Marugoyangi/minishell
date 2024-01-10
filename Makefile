# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/05 14:27:11 by jeongbpa          #+#    #+#              #
#    Updated: 2024/01/10 11:28:15 by jeongbpa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR = ./srcs/
BONUS_DIR = ./bonus_srcs/

SRCS =	$(addprefix $(SRC_DIR), minishell.c \
								utils/ascii_art.c utils/node_ctl.c utils/memory_ctl.c utils/general_utils.c \
								utils/expand_utils.c utils/line_ctl.c utils/line_split.c utils/sort_utils.c utils/heredoc_utils.c\
								parse/tokenize.c parse/lexicize.c parse/parser.c parse/syntax_check.c\
								parse/syntax_cmd.c parse/parsing_sort.c \
								expand/expand.c expand/expand_asterisk.c expand/asterisk_filter.c expand/expand_heredoc.c\
								expand/expand_split.c expand/asterisk_utils.c \
								initialize/envp_init.c \
								initialize/init.c initialize/signal.c initialize/terminal.c \
								error/error.c \
								exec/exec_set_start.c \
								exec/exec_logical_operator.c exec/exec_pipeline.c exec/exec_subshell.c \
								exec/exec_redirection.c exec/exec_redirection_utils.c exec/exec_redirection_built_in.c \
								exec/exec_simple_command.c exec/exec_simple_command_utils.c \
								exec/exec_built_in.c exec/exec_built_in_cd.c \
								exec/exec_built_in_env.c exec/exec_built_in_env_utils.c exec/exec_built_in_export.c \
								exec/exec_heredoc.c exec/exec_heredoc_file.c)

BONUS_SRCS = $(addprefix $(BONUS_DIR), minishell_bonus.c \
								utils/ascii_art_bonus.c utils/node_ctl_bonus.c utils/memory_ctl_bonus.c utils/general_utils_bonus.c \
								utils/expand_utils_bonus.c utils/line_ctl_bonus.c utils/line_split_bonus.c utils/sort_utils_bonus.c utils/heredoc_utils_bonus.c\
								parse/tokenize_bonus.c parse/lexicize_bonus.c parse/parser_bonus.c parse/syntax_check_bonus.c\
								parse/syntax_cmd_bonus.c parse/parsing_sort_bonus.c \
								expand/expand_bonus.c expand/expand_asterisk_bonus.c expand/asterisk_filter_bonus.c expand/expand_heredoc_bonus.c\
								expand/expand_split_bonus.c expand/asterisk_utils_bonus.c \
								initialize/envp_init_bonus.c \
								initialize/init_bonus.c initialize/signal_bonus.c initialize/terminal_bonus.c \
								error/error_bonus.c \
								exec/exec_set_start_bonus.c \
								exec/exec_logical_operator_bonus.c exec/exec_pipeline_bonus.c exec/exec_subshell_bonus.c \
								exec/exec_redirection_bonus.c exec/exec_redirection_utils_bonus.c exec/exec_redirection_built_in_bonus.c \
								exec/exec_simple_command_bonus.c exec/exec_simple_command_utils_bonus.c \
								exec/exec_built_in_bonus.c exec/exec_built_in_cd_bonus.c \
								exec/exec_built_in_env_bonus.c exec/exec_built_in_env_utils_bonus.c exec/exec_built_in_export_bonus.c \
								exec/exec_heredoc_bonus.c exec/exec_heredoc_file_bonus.c)

OBJS =	$(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

HEADER = minishell.h
BONUS_HEADER = minishell_bonus.h
MAKEFILE = Makefile
NAME = minishell
LIBFT = ./libft/libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes/

ifeq ($(BONUS_FLAG), 42)
	OBJ_FLAG = $(BONUS_OBJS)
	OBJ_EXCEPT = $(OBJS)
	OBJ_HEADER = $(BONUS_HEADER)
else
	OBJ_FLAG = $(OBJS)
	OBJ_EXCEPT = $(BONUS_OBJS)
	OBJ_HEADER = $(HEADER)
endif

%.o: %.c $(OBJ_HEADER)
	 $(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME): $(OBJ_FLAG) $(LIBFT)
	@rm -f $(OBJ_EXCEPT)
	$(CC) $^ -L./libft/ -lft -lreadline -lncurses -o $(NAME)

$(LIBFT):
	@make -C libft

all: $(NAME)

bonus:
	@make BONUS_FLAG=42 $(NAME)

re: fclean all

clean:
	rm -f $(OBJS) $(BONUS_OBJS)
	@$(MAKE) -C ./libft fclean

fclean: clean
	rm -f $(NAME)

.PHONY: all clean fclean re bonus
