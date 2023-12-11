/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:35 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/11 15:28:10 by jeongbpa         ###   ########.fr       */
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
	char			quote;
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
	t_line	line;
	t_node	*ast_head;
	t_env	*envp_head;
	t_error	*error;
	int		last_exit_status;
	int		is_interactive;
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
# define T_PARENTHESIS		7
# define T_ASTERISK			8
# define T_SPACE 			9

// error type
# define E_MALLOC			-2
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

void	test_print_list(t_node *root);
void	tokenize(t_line *line, t_arg *arg);
void    print_ascii(void);
void	print_ascii2(void);
void	parser(t_arg *arg);

void	*ft_malloc(int size);
void	ft_free(void *ptr);
void	free_split(void **ptr);
void	free_node(t_node *node);
void	free_env(t_env *env);
void	free_arg(t_arg *arg);
void	free_ast(t_node *node);
char	*modified_strtrim(char const *s1, char const *set);
char	*modified_strdup(char *s1);
char	*modified_substr(char *s, int start, int len);
char	*modified_strjoin(char *s1, char *s2, int free);

int ft_strlen(const char *str);
char	*ft_substr(char const *s, int start, int len);
char* ft_strdup(char *s1);
char* ft_strtrim(char *s, char const *c);
t_line	*subline(t_line *line, int start, int end);

void	lexicize(t_arg *arg);
t_node	*create_node(char *data, t_line *line, int type);
t_line	*get_line_info(char *env);

int ft_strcmp(const char *s1, const char *s2);

int replace_line(t_line *data, t_line **line, int start, int end);

void	expand_tilde(t_line **line, t_arg *arg);
void	expand_asterisk(t_line **line);
t_node	**filter_asterisk(char **line);
char	**line_split(char *line, char c);
char	*ft_strjoin(char const *s1, char const *s2);
void	asterisk_subdir(t_node **result, char **line, char *pwd, int *depth);
t_node	*last_node(t_node *node);

void expand_vars(t_arg *arg);
void	remove_quotes(t_line **line);
int	ft_delete_line(int len, t_line **line, int start);
int	ft_count_words(char *s, char c);
int	add_line(t_line *data, t_line *line, int start, int end);
t_line	*node_to_line(t_node *node);

char	**ft_split(char *s, char c);
char	*ft_itoa(int n);
t_env	*init_envp(char **envp);

char	*find_env(t_env *envp, char *key);
void	init_shell_vars(t_arg *arg);

char	**line_to_data(t_line *line, int start, int end);

t_node	*create_ast_node(t_line *line, int start, int end, int type);
void	error_handler(t_arg *arg);
int	syntax_subshell(t_node *head, t_arg *arg);
int	check_syntax(t_node *head, t_arg *arg, int flag);
t_node  **sort_node(t_node *root, int type);
t_node	*sort_redirection(t_node **root);
void	sort_free(t_node **root);
t_node	*append_cmd(t_node *root, int type);
void	split_expanded(t_node *node);
void	append_subshell(t_node *root);

void	terminal_init(t_arg *arg, struct termios *term, struct termios *original_term, char **envp);
void terminal_default(struct termios *original_term, int exit);
void terminal_interactive(struct termios *term);
void signal_default(void);
void signal_interactive(void);
char	*get_ps1(t_arg *arg);



// 실행부 함수
// 실행부의 모든 함수는 정상종료시 0, 그 외의 경우 1 및 종료 status를 반환한다.
// ft_itoa.c
char	*ft_itoa(int n);
int		ft_countdigit(int n);
void	ft_nbrdup(int n, int digit, char *nbr);

// exec_libft.c
// int	ft_strcmp(const char *s1, const char *s2);

// exec_set_start.c
void	set_exec(t_arg *arg);
void	set_heredoc(t_node *node);
char	**set_path(t_env *env);
int		start_exec(t_node *node, t_env *env, char **path);

// exec_simplecommand.c
int		exec_command(t_node *node, t_env *env, char **path);
int		external_command(t_node *node, char **path);
char	*find_path(char **path, char *command);
int		check_built_in(t_node *node, t_env *env);

// exec_built_in.c
int		built_in_echo(char **argv);
int		built_in_cd(char **argv);
int		built_in_pwd();
void	built_in_exit();

// exec_built_in_env.c
int	built_in_export(t_node *node , t_env *env);
int	export_none_arg(t_env *env);
int	built_in_unset(t_node *node , t_env *env);
int	built_in_env(t_env *env);

// exec_built_in_env_utils.c
t_env	*dup_list(t_env *env);
void	sort_list(t_env *env);
void	free_list(t_env *env);

// exec_logical_subshell_pipe.c
int		exec_logical_operator(t_node *node, t_env *env, char **path);
int		exec_subshell(t_node *node, t_env *env, char **path);
int		exec_pipeline(t_node *node, t_env *env, char **path);
void	exec_pipe_child1(t_node *node, t_env *env, char **path, int *fd);
void	exec_pipe_child2(t_node *node, t_env *env, char **path, int *fd);

// exec_redirection.c
int exec_redirection(t_node *node, t_env *env, char **path);
int exec_output(t_node *node, t_env *env, char **path);
int exec_append(t_node *node, t_env *env, char **path);
int exec_input(t_node *node, t_env *env, char **path);
int exec_heredoc(t_node *node, t_env *env, char **path);

void	print_env(t_env *env);


#endif
