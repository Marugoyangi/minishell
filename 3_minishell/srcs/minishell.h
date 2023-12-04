/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:35 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/05 06:33:10 by jeongbpa         ###   ########.fr       */
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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;


typedef struct s_node
{
	int				type;
	char			*data;
	char			**argv;
	struct s_node	*left;
	struct s_node	*right;
}				t_node;

typedef struct s_arg
{
	t_line	line;
	t_node	*ast_head;
	t_env	*envp_head;
	int		last_exit_status;
	char	*pwd;
	char	*oldpwd;
	char	*tilde;
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
# define T_ASTERISK			9
# define T_SPACE 			10

// tree direction
# define LEFT				0
# define RIGHT				1

void	test_print_list(t_node *root);
void	check_line(t_arg *arg);
void print_ascii(void);

void	parser(t_arg *arg);

int ft_strlen(const char *str);
char	*ft_substr(char const *s, int start, int len);
char* ft_strdup(const char *s1);
char* ft_strtrim(char const*s, char const *c);

void	lexicize(t_arg *arg);
t_node	*create_node(char *data, int type, int *line_info_index);

int ft_strcmp(const char *s1, const char *s2);

int replace_line(char *value, t_line *line, int start, int end);

void	expand_tilde(int *position, t_line *line, t_arg *arg);
void	expand_asterisk(int *position, t_line *line);
t_node	**filter_asterisk(char **line);
char	**line_split(char *line, char c);
char	*ft_strjoin(char const *s1, char const *s2);
void	asterisk_subdir(t_node **result, char **line, char *pwd, int *depth);
t_node	*last_node(t_node *node);

void expand_vars(int *position, t_line *line, t_arg *arg);
void	remove_quotes(int *position, t_line *line);
int	ft_delete_line(int len, t_line *line, int start);
int	ft_count_words(char *s, char c);
int	add_line(t_line *data, t_line *line, int start, int end);
t_line	*node_to_line(t_node *node);

char	**ft_split(char const *s, char c);

t_env	*init_envp(char **envp);

char	*find_env(t_env *envp, char *key);
void	init_shell_vars(t_arg *arg);

#endif
