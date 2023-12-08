/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:18:35 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/08 11:25:31 by jeongbpa         ###   ########.fr       */
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
	char	*ps1;
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

//syntax flag
# define S_OPERATOR			0
# define S_SIMPLE_COMMAND	0
# define S_SUBSHELL			0

void	test_print_list(t_node *root);
void	check_line(t_arg *arg);
void print_ascii(void);

void	parser(t_arg *arg);

int ft_strlen(const char *str);
char	*ft_substr(char const *s, int start, int len);
char* ft_strdup(const char *s1);
char* ft_strtrim(char const*s, char const *c);

void	lexicize(t_arg *arg, int flag);
t_node	*create_node(char *data, int type, int *i);

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
void	split_red(t_node *node);
void	append_subshell(t_node *root);

void	terminal_init(t_arg *arg, struct termios *term, struct termios *original_term, char **envp);
void terminal_default(struct termios *term, struct termios *original_term, int exit);
void terminal_interactive(struct termios *term);
void signal_default(void);
void signal_interactive(void);
char	*get_ps1(t_arg *arg);


// 실행부 함수
// 0.c
// 기타 유틸 함수
int	ft_strcmp(const char *s1, const char *s2);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen2(const char *s);

// 1.c
// 실행부 사전 준비
// execve를 위한 경로 정리 작업, 히어독 파일 생성, 노드 분기점
void    set_heredoc(t_node *node);
char	**set_path(t_env *env);
void    start_exec(t_node *node, t_env *env, char **path);
void    set_exec(t_arg *__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2_ARGLIST);

// 2.c
// 심플커맨드 execve로 실행
char	*find_path(char **path, char *command);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen2(const char *s);
void	exec_command(t_node *node, t_env *env, char **path);

// 3.c
// 빌트인 기능 구현
void	built_in_cd(char **argv);
void    built_in_pwd();
void    exec_exit();
void    built_in_echo(char **argv);

// 4.c
// 환경변수 관련 기능 구현
void	sort_list(t_env *env);
void	free_list(t_env *env);
void	built_in_export(t_node *node , t_env *env);
void	built_in_unset(t_node *node , t_env *env);
void	built_in_env(t_env *env);
t_env *dup_list(t_env *env);

// 5.c
// 서브쉘과 파이프라인
// 서브쉘은 단일 포크 생성 자식 프로세스에서 실행, 부모프로세스 대기
// 파이프는 포크 2번으로 병렬구조 실행, 부모프로세스는 대기만
void	exec_subshell(t_node *node, t_env *env, char **path);
void	exec_pipeline(t_node *node, t_env *env, char **path);
void	exec_logical_operator(t_node *node, t_env *env, char **path);

// 6.c
// 리다이렉션 분기점
// 각 리다이렉션에 맞는 fd 연결 후 다음 노드 실행.
void	exec_output(t_node *node, t_env *env, char **path);
void	exec_append(t_node *node, t_env *env, char **path);
void	exec_input(t_node *node, t_env *env, char **path);
void	exec_heredoc(t_node *node, t_env *env, char **path);
void	exec_redirection(t_node *node,t_env *env, char **path);




#endif
