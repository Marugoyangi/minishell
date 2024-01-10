/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:35 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/08 20:21:58 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

# include <errno.h>			// perror
# include <stdio.h>			// printf, strerror
# include <stdlib.h>		// malloc, free, fork, exit, getenv
# include <unistd.h>		// write, access, read, getcwd, chdir, unlink, pipe
							//  execve, dup, dup2, isatty, ttyname, ttyslot
# include <fcntl.h>			// open, close
# include <signal.h>		// kill, signal, sigaction, sigemptyset, sigaddset
# include <dirent.h>		// opendir, readdir, closedir
# include <termios.h>		// tcsetattr, tcgetattr, 
# include <term.h>			// tgetent, tgetflag, tgetnum, tgetstr
# include <ncurses.h>		// tgoto, tputs
# include <readline/readline.h> // readline, rl_clear_history, rl_on_new_line,
								// rl_replace_line, rl_redisplay
# include <readline/history.h>	// add_history
# include <sys/wait.h>		// wait, waitpid, wait3, wait4
# include <sys/ioctl.h>		// ioctl
# include <sys/stat.h>		// stat, lstat, fstat

// libft
# include "../libft/libft.h"

// the signal number recieved
extern int	g_recived_signal;

typedef struct s_line
{
	char	*data;
	char	*info;
}			t_line;

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
	t_line			*line;
	char			*filename;
	char			**argv;
	struct s_node	*left;
	struct s_node	*right;
}				t_node;

typedef struct s_redirection
{
	t_node	*exec_node;
	t_node	*input_node;
	t_node	*output_node;
}			t_redirection;

typedef struct s_error
{
	int		code;
	int		type;
	char	*token;
}			t_error;

typedef struct s_arg
{
	t_line			line;
	t_node			*ast_head;
	t_env			*envp_head;
	t_error			*error;
	struct termios	term;
	struct termios	original_term;
	int				last_exit_status;
	int				is_subshell;
	int				fork_sign;
	char			*minishell_path;
	char			*pwd;
	char			*oldpwd;
	char			*tilde;
	char			*ps[2];
}					t_arg;

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
# define T_PARENTHESIS		7
# define T_ASTERISK			8
# define T_SPACE 			9

// error type
# define E_MALLOC			1
# define E_WRONG_SYNTAX		-3
# define E_FD				-4
# define E_NO_CMD			-5
# define E_WRONG_ENV		-6
# define E_EXIT_NOT_NUMERIC	-7
# define E_CD_NO_FILE		-8
# define E_TOO_MANY_ARG		-9
# define E_UNCLOSED_QUOTES	-10

//syntax flag
# define S_OPERATOR			0
# define S_SIMPLE_COMMAND	0
# define S_SUBSHELL			0

//coloring ANSI escape code
# define C_ARROW			"\001\033[32m➜\002"
# define C_PROMPT			"\001\033[36m\002"
# define C_RESET			"\001\033[0m\002"

//------------------------------//
//		parsing functions		//
//------------------------------//

// Tokenization
void	tokenize(t_line *line, t_arg *arg);
void	parser(t_arg *arg);
void	lexicize(t_arg *arg);
void	expand_vars(t_arg *arg);
t_node	*create_ast_node(t_line *line, int start, int end, int type);

// Memory Management
void	error_handler(t_arg *arg, int type);
void	*ft_malloc(int size);
void	free_split(char **ptr);
void	free_node(t_node *node);
void	free_env(t_env *env);
void	free_arg(t_arg *arg);
void	free_ast(t_node *node);
void	free_line(t_line *line);

// String Manipulation
char	*modified_strtrim(char *s1, char const *set);
char	*modified_strdup(char *s1);
char	*modified_substr(char *s, int start, int len);
char	*modified_strjoin(char *s1, char *s2, int free);

// Environment Variables
void	set_env(t_env *env_head, char *key, char *value);
void	append_env(t_env *env, char *key, char *value);

// Line Manipulation
t_node	*create_node(char *data, t_line *line, int type);
t_node	**filter_asterisk(char **line);
t_node	*last_node(t_node *node);
t_line	*get_info(char *env, int flag);
t_line	*node_to_line(t_node *node);
t_line	*subline(t_line *line, int start, int end);
int		replace_line(t_line *data, t_line **line, int start, int end);
char	**line_split(char *line, char c);
char	*set_heredoc_filename(int *i);
void	remove_quotes(t_line **line);
void	expand_envp(t_line **line, t_arg *arg);
void	expand_tilde(t_line **line, t_arg *arg);
void	expand_asterisk(t_line **line, int index);
void	asterisk_subdir(t_node **result, char **line, char *pwd, int *depth);
int		ft_delete_line(int len, t_line **line, int start, int flag);
int		ft_count_words(char *s, char c);
int		ft_delete_line_free(t_line **line);

// Environment Initialization
t_env	*init_envp(char **envp);
char	*find_env(t_env *envp, char *key);
void	init_shell_vars(t_arg *arg);
char	*get_ps(t_arg *arg, int no);
void	init_ps(t_arg *arg);

// Syntax Checking
t_node	**sort_node(t_node *root, int type);
t_node	*sort_redirection(t_node **root);
t_node	*append_cmd(t_node *root, int type);
int		syntax_subshell(t_node *head, t_arg *arg);
int		check_syntax(t_node *head, t_arg *arg, int flag);
void	sort_free(t_node **root);
void	split_expanded(t_node *node);
void	append_subshell(t_node *root);

// Terminal Initialization and Handling
void	terminal_init(t_arg *arg, char **envp, char **argv);
void	terminal_default(t_arg *arg);
void	terminal_interactive(t_arg *arg);
void	signal_default(void);
void	signal_interactive(void);

// Signal Handlers
void	sig_handler_heredoc(int signo);
void	sig_handler(int signum);
void	sig_handler_exec(int signum);
void	sig_handler_waiting(int signum);
void	signal_ignore(void);
void	waitpid_signal(t_arg *arg, int *status);

// Heredoc Expansion
void	expand_heredoc(t_arg *arg);
void	get_heredoc(t_arg *arg);
void	get_heredoc_filename(t_node *root, int *i, t_arg *arg);
void	heredoc_variables_init(int *status, int *pid, int *xy);

// Miscellaneous
void	check_subshell(t_node *node, t_error *error, int *type, t_arg *arg);
void	check_command(t_node *node, t_error *error, int *found_type);
void	set_minishell_path(t_arg *arg, char *argv);
void	print_ascii(void);
void	print_ascii2(void);
int		filter_utf8(const char *str);
int		ps_len(char *ps);
int		is_number(char c);
int		check_double_command(int *found_type, int index);
int		is_not_delimiter(char c);
int		node_type_numbers(t_node *root, int type);
int		check_asterisk(char **filter, char *file, int *depth);
int		asterisk_exceptions(DIR *dir, char **line, int *depth);

//------------------------------//
//		excution functions		//
//------------------------------//

// ft_itoa.c
char	*ft_itoa(int n);
int		ft_countdigit(int n);
void	ft_nbrdup(int n, int digit, char *nbr);

// exec_libft.c
int		ft_atoi(const char *nptr);
int		ft_strncmp(const char *s1, const char *s2, int n);

// exec_set_start.c
void	set_exec(t_arg *arg);
int		start_exec(t_node *node, t_arg *arg);

// exec_logical_operator.c
int		exec_logical_operator(t_node *node, t_arg *arg);

// exec_pipeline.c
int		exec_pipeline(t_node *node, t_arg *arg);
int		fail_to_fork(t_arg *arg, int pid);
int		exec_parent(t_arg *arg, int *pid, int *pipe);
void	exec_pipe_child1(t_node *node, t_arg *arg, int *pipe);
void	exec_pipe_child2(t_node *node, t_arg *arg, int *pipe);

// exec_redirection.c
int		exec_redirection(t_node *node, t_arg *arg);
int		check_built_in_redirection(t_node *node);
int		built_in_redirection_fd(t_redirection *node, t_arg *arg, int *fd);
void	exec_redirection_child(t_redirection *node, t_arg *arg, int *fd);
void	exec_redirection_parent(t_arg *arg, pid_t pid, int *status);

// exec_redirection_built_in.c
void	dup_with_close(int fd1, int fd2);
int		check_built_in_redirection(t_node *node);
int		built_in_redirection_fd(t_redirection *node, t_arg *arg, int *fd);
int		built_in_redirection_fd2(t_redirection *node, t_arg *arg);

// exec_redirection_utils.c
t_node	*get_input_node(t_node *node, int *fd);
t_node	*get_output_node(t_node *node, int *fd);
int		get_input_fd(t_node *node);
int		get_output_fd(t_node *node);
int		error_input_fd(t_node *node);

// exec_subshell.c
int		exec_subshell(t_node *node, t_arg *arg);
void	set_subshell(t_node *node, t_arg *arg);

// exec_simple_command.c
int		exec_command(t_node *node, t_arg *arg);
int		external_command(t_node *node, t_arg *arg, char **path);
void	external_command_child(t_node *node, t_arg *arg, char **path);
void	exec_check_path(t_node *node, t_arg *arg, char **path);

// exec_simplecommand_utils.c
int		exec_perror(char *str, int sign);
char	**set_path(t_env *env);
char	*find_path(char **path, char *command);
char	**make_envp(t_env *envp_head);

// exec_built_in.c
int		check_built_in(t_node *node, t_arg *arg);
int		built_in_echo(char **argv);
int		built_in_pwd(t_arg *arg);
int		built_in_exit(t_node *node, t_arg *arg);

// exec_built_in_cd.c
int		built_in_cd(t_node *node, t_arg *arg, char **argv);
int		built_in_cd_oldpwd(t_node *node, t_arg *arg);
void	built_in_cd_set_pwd_arg(char *tmp, t_arg *arg, t_node *node);
void	built_in_cd_set_pwd_env(t_node *node, t_arg *arg);

// exec_built_in_env.c
int		built_in_unset(t_node *node, t_arg *arg);
void	built_in_unset_iter(t_node *node, t_env	*cur, t_arg *arg, int i);
int		built_in_env(t_node *node, t_env *env);
int		built_in_export(t_node *node, t_env *env);

// exec_built_in_env_utils.c
t_env	*find_env_node(t_env *env, char *key);
t_env	*dup_list(t_env *env);
void	sort_list(t_env *env);
void	free_list(t_env *env);
void	free_env_node(t_env *env);

// exec_built_in_export.c
int		check_arg_name(char *str, int *sign);
int		export_none_arg(t_env *env);
int		export_arg(t_node *node, t_env *env, int i, int sign);
void	export_arg_equal(t_env *env, char **tmp);
void	export_arg_not_equal(t_env *env, char *arg);

// exec_heredoc.c
void	init_file_for_heredoc(t_node *node, int *row, int *i, t_arg *arg);
void	set_heredoc(t_node *node, t_arg *arg, int *row, int *i);
void	get_heredoc(t_arg *arg);
void	init_file_heredoc_iter(int *i, int *row, char *line, int fd);
char	*find_heredoc_prefix(t_arg *arg, int *i, int *row);

// exec_heredoc_file.c
char	*set_heredoc_filename(int *i);
void	get_heredoc_filename(t_node *root, int *i, t_arg *arg);
#endif
