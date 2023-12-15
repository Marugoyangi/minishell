/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:35 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/16 02:23:29 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

extern int g_signal_fork;

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
	int				is_interactive;
	int				is_subshell;
	int				fork_sign;
	char			*pwd;
	char			*oldpwd;
	char			*tilde;
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


// parsing functions
void	*ft_malloc(int size);
void	test_print_list(t_node *root);
void	tokenize(t_line *line, t_arg *arg);
void	print_ascii(void);
void	print_ascii2(void);
void	parser(t_arg *arg);
int		is_number(char c);
char	*ft_strchr(const char *s, int c);
void	sig_handler_heredoc(int signo);
void	sig_handler(int signum);
void	sig_handler_exec(int signum);
int		ft_strstr(char *str, char *to_find);
void	expand_envp(t_line **line, t_arg *arg);
void	get_heredoc_filename(t_node *root, int *i, t_arg *arg);
char	*set_heredoc_filename(int *i);
void	terminal_noninteractive(t_arg *arg);
void	free_split(char **ptr);
void	free_node(t_node *node);
void	free_env(t_env *env);
void	free_arg(t_arg *arg);
void	free_ast(t_node *node);
char	*modified_strtrim(char const *s1, char const *set);
char	*modified_strdup(char *s1);
char	*modified_substr(char *s, int start, int len);
char	*modified_strjoin(char *s1, char *s2, int free);
void	set_env(t_env *env_head, char *key, char *value);
void	append_env(t_env *env, char *key, char *value);
int 	ft_strlen(const char *str);
char	*ft_substr(char const *s, int start, int len);
char	*ft_strdup(char *s1);
char	*ft_strtrim(char *s, char const *c);
t_line	*subline(t_line *line, int start, int end);
void	lexicize(t_arg *arg);
t_node	*create_node(char *data, t_line *line, int type);
t_line	*get_line_info(char *env);
int 	ft_strcmp(const char *s1, const char *s2);
int		replace_line(t_line *data, t_line **line, int start, int end);
void	expand_tilde(t_line **line, t_arg *arg);
void	expand_asterisk(t_line **line, int index);
t_node	**filter_asterisk(char **line);
char	**line_split(char *line, char c);
char	*ft_strjoin(char const *s1, char const *s2);
void	asterisk_subdir(t_node **result, char **line, char *pwd, int *depth);
t_node	*last_node(t_node *node);
void 	expand_vars(t_arg *arg);
void	remove_quotes(t_line **line);
int		ft_delete_line(int len, t_line **line, int start);
int		ft_count_words(char *s, char c);
int		add_line(t_line *data, t_line *line, int start, int end);
t_line	*node_to_line(t_node *node);
char	**ft_split(char *s, char c);
char	*ft_itoa(int n);
t_env	*init_envp(char **envp);
char	*find_env(t_env *envp, char *key);
void	init_shell_vars(t_arg *arg);
char	**line_to_data(t_line *line, int start, int end);
t_node	*create_ast_node(t_line *line, int start, int end, int type);
void	error_handler(t_arg *arg, int type);
int		syntax_subshell(t_node *head, t_arg *arg);
int		check_syntax(t_node *head, t_arg *arg, int flag);
t_node  **sort_node(t_node *root, int type);
t_node	*sort_redirection(t_node **root);
void	sort_free(t_node **root);
t_node	*append_cmd(t_node *root, int type);
void	split_expanded(t_node *node);
void	append_subshell(t_node *root);
void	terminal_init(t_arg *arg, char **envp, char **argv);
void	terminal_default(int exit, t_arg *arg);
void 	terminal_interactive(t_arg *arg);
void 	signal_default(void);
void 	signal_interactive(void);
void	expand_heredoc(t_arg *arg);
void 	get_heredoc(t_arg *arg);
int		ft_strlcpy(char *dest, const char *src, int size);
int	filter_utf8(const char	*str);
int		ps_len(char *ps);
int		check_double_command(int *found_type, int index);
void	check_subshell(t_node *node, t_error *error, t_arg *arg, int *type);
void	check_command(t_node *node, t_error *error, int *found_type);
int		is_not_delimiter(char c);
int		is_number(char c);

// excution functions
// ft_itoa.c
char	*ft_itoa(int n);
int		ft_countdigit(int n);
void	ft_nbrdup(int n, int digit, char *nbr);

// exec_libft.c
int		ft_atoi(const char *nptr);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

// exec_set_start.c
void	set_exec(t_arg *arg);
void	set_heredoc(t_node *node, t_arg *arg, int *count, int *i);
int		start_exec(t_node *node, t_arg *arg);

// exec_simplecommand.c
int		exec_command(t_node *node, t_arg *arg);
int		check_built_in(t_node *node, t_arg *arg);
char	**set_path(t_env *env);
int		external_command(t_node *node, t_arg *arg, char **path);
char	*find_path(char **path, char *command);

// exec_built_in.c
int		built_in_echo(char **argv);
int		built_in_cd(char **argv);
int		built_in_pwd();
void	built_in_exit(t_node *node);

// exec_built_in_env.c
int	built_in_export(t_node *node , t_env *env);
int	export_none_arg(t_env *env);
int	built_in_unset(t_node *node , t_arg *arg);
int	built_in_env(t_env *env);

// exec_built_in_env_utils.c
t_env	*dup_list(t_env *env);
void	sort_list(t_env *env);
void	free_list(t_env *env);
void	free_env_node(t_env *env);

// exec_logical_subshell_pipe.c
int		exec_logical_operator(t_node *node, t_arg *arg);
int		exec_subshell(t_node *node, t_arg *arg);
int		exec_pipeline(t_node *node, t_arg *arg);
int		exec_parent(t_arg *arg, int *pid, int *pipe);
void	exec_pipe_child1(t_node *node, t_arg *arg, int *fd);
void	exec_pipe_child2(t_node *node, t_arg *arg, int *fd);

// exec_redirection.c
int exec_redirection(t_node *node, t_arg *arg);
int exec_output(t_node *node, t_arg *arg);
int exec_append(t_node *node, t_arg *arg);
int exec_input(t_node *node, t_arg *arg);
int exec_heredoc(t_node *node, t_arg *arg);
void	exec_redirection_child(t_node *node, t_arg *arg, int fd, int io);
t_node	*get_redirection_node(t_node *node);

// exec_redirection_utils.c
int	check_built_in_redirection(t_node *node);
int	external_command_redirection(t_node *node, t_arg *arg, int fd, int fd_sign);

int	exec_perror(char *str);
void	exec_check_path(t_node *node, t_arg *arg, char **path);
int	single_redirection(t_node *node, t_arg *arg);
char **make_envp(t_env *envp_head);
void	set_subshell(t_node *node, t_arg *arg);

#endif
