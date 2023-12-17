/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 10:46:56 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/17 23:01:55 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
	int	j;

	i = 1;
	while (argv[i] && !strncmp(argv[i], "-n", 2))
	{
		j = 1;
		while (argv[i][j] == 'n')
			j++;
		if (argv[i][j])
			break ;
		i++;
	}
	if (i > 1)
		j = 0;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (j)
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
