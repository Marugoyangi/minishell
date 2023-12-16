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

int	check_built_in(t_node *node, t_arg *arg)
{
	if (!ft_strcmp(node->data, "echo"))
		return (built_in_echo(node->argv));
	else if (!ft_strcmp(node->data, "cd"))
		return (built_in_cd(arg->envp_head, node->argv));
	else if (!ft_strcmp(node->data, "pwd"))
		return (built_in_pwd());
	else if (!ft_strcmp(node->data, "exit"))
		built_in_exit(node);
	else if (!ft_strcmp(node->data, "export"))
		return (built_in_export(node, arg->envp_head));
	else if (!ft_strcmp(node->data, "unset"))
		return (built_in_unset(node, arg));
	else if (!ft_strcmp(node->data, "env"))
		return (built_in_env(arg->envp_head));
	return (-1);
}

int	built_in_echo(char **argv)
{
	int	i;

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

int	built_in_pwd(void)
{
	char	*current_path;

	current_path = getcwd(0, 0);
	if (!current_path)
	{
		perror("minishell");
		return (1);
	}
	printf("%s\n", current_path);
	free(current_path);
	return (0);
}

void	built_in_exit(t_node *node)
{
	int	exit_num;
	int	i;

	exit_num = 0;
	i = 0;
	if (node->argv[1])
	{
		while (node->argv[1][i] >= '0' && node->argv[1][i] <= '9')
			i++;
		if (node->argv[1][i])
		{
			printf("minishell: exit: numeric argument required");
			exit (255);
		}
	}
	if (node->argv[1] && node->argv[2])
	{
		printf("minishell: exit: too many arguments\n");
		exit (1);
	}
	if (node->argv[1])
		exit_num = ft_atoi(node->argv[1]);
	exit(exit_num);
}
