/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:35 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/11/29 18:28:38 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h> 		// printf, strerror
# include <stdlib.h>		// malloc, free, fork, exit, getenv
# include <unistd.h>		// write, access, read, getcwd, chdir, unlink, pipe
							//  execve, dup, dup2, isatty, ttyname, ttyslot
# include <fcntl.h>			// open, close
# include <signal.h>		// kill, signal, sigaction, sigemptyset, sigaddset
# include <dirent.h>		// opendir, readdir, closedir
# include <termios.h>		// tcsetattr, tcgetattr, 
# include <term.h>			// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <readline/readline.h> // readline, rl_clear_history, rl_on_new_line,
								// rl_replace_line, rl_redisplay
# include <readline/history.h>	// add_history
# include <sys/wait.h>		// wait, waitpid, wait3, wait4
# include <sys/ioctl.h>		// ioctl
# include <sys/stat.h>		// stat, lstat, fstat

typedef struct s_line
{
	char	*data;
	char	*info;
}			t_line; // 원본이랑 진짜 문자인지 저장

typedef struct s_node
{
	int				type;
	char			*data;
	struct s_node	*left;
	struct s_node	*right;
}				t_node;

typedef struct s_arg
{
	t_line	input;
	t_node	*ast_head;
	t_node	*envp_head;
}			t_arg;

// lexer type
# define L_LOGICAL_OPERATOR	1
# define L_PIPELINE			2
# define L_SIMPLE_COMMAND	3
# define L_REDIRECTION		4
# define L_SUBSHELL			5

// token type
# define T_WORD				1
# define T_OPERATOR			2
# define T_SINGLE_QUOTE 	3
# define T_DOUBLE_QUOTE 	4
# define T_LITERAL			5
# define T_ENV				6
# define T_ENV_QUOTED		7
# define T_PARENTHESIS		8
# define T_SPACE 			9

void	check_line(t_arg *arg);
void print_ascii(void);

int ft_strlen(const char *str);
char	*ft_substr(char const *s, int start, int len);
char* ft_strdup(const char *s1);
char* ft_strtrim(char const*s, char const *c);

void	lexicize(t_arg *arg);

t_node	*create_node(char *data, int type, int *line_info_index);

#endif