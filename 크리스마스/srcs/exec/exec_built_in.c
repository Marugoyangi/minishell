/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   exec_built_in.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: seungwok <seungwok@student.42seoul.kr>	 +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/12/04 15:43:41 by seungwok		  #+#	#+#			 */
/*   Updated: 2023/12/08 19:06:03 by seungwok		 ###   ########seoul.kr  */
/*																			*/
/* ************************************************************************** */

#include "../minishell.h"

int		built_in_echo(char **argv);
int		built_in_cd(char **argv);
int		built_in_pwd();
void	built_in_exit(t_node *node);

// echo, 입력한 인수를 그대로 출력
// -n 옵션 사용시 개행 미출력
int	built_in_echo(char **argv)
{
	int i;

	i = 1;
	if (!argv)
		return (0);
	if (argv[1] && !strcmp(argv[1], "-n"))
		i = 2;
	while (argv[i])
	{
		if (argv[1] && !strcmp(argv[1], "-n") && i > 2)
			printf(" ");
		else if (argv[1] && strcmp(argv[1], "-n") && i > 1)
			printf(" ");
		printf("%s", argv[i]);
		i++;
	}
	if (argv[1] && strcmp(argv[1], "-n"))
		printf("\n");
	return (0);
}

// 상대경로, 절대경로만 인식하는 cd
// 애초에 chdir이 상대경로던 절대경로던 알아서 인식한다.
int	built_in_cd(char **argv)
{
	if (chdir(argv[1]) == -1)
	{
		perror("chdir");
		return (1);
	}
	return (0);
}

// 현재 작업 디렉토리의 경로를 출력
int	built_in_pwd()
{
	char	*current_path;

	// buffersize에 NULL 입력시 사이즈를 알아서 인식한다.
	// getcwd는 실패시 NULL을 반환한다.
	current_path = getcwd(0, 0);
	if (!current_path)
	{
		perror("getcwd");
		return (1);
	}
	printf("%s\n", current_path);
	free(current_path);
	return (0);
}

// 쉘을 종료.
void	built_in_exit(t_node *node)
{
	int exit_num;

	exit_num = 0;
	if (node->argv[1])
		exit_num = ft_atoi(node->argv[1]);
	exit(exit_num);
}
