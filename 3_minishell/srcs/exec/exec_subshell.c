/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 12:32:45 by woopinbell        #+#    #+#             */
/*   Updated: 2023/12/15 14:24:21 by woopinbell       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		exec_subshell(t_node *node, t_arg *arg);
void	set_subshell(t_node *node, t_arg *arg);

int	exec_subshell(t_node *node, t_arg *arg)
{
	pid_t	pid;
	int		status;
	char	**path;	

	pid = 0;
	status = 0;
	arg->fork_sign++;
	pid = fork();
	if (!pid)
	{
		path = set_path(arg->envp_head);
		set_subshell(node, arg);
		exec_check_path(node, arg, path);
	}
	else
	{
		waitpid(pid, &status, 0);
		arg->fork_sign--;
	}
	return (status);
}

void	set_subshell(t_node *node, t_arg *arg)
{
	int		i;
	char	*tmp1;
	char	**tmp2;

	node->data = modified_strtrim(node->data, "(");
	node->data = modified_strtrim(node->data, ")");
	tmp1 = ft_strjoin(arg->minishell_path, " /n ");
	tmp1 = ft_strjoin(tmp1, node->data);
	free(node->data);
	node->data = "./minishell";
	tmp2 = ft_split(tmp1, ' ');
	i = -1;
	free(tmp1);
	i = -1;
	while (node->argv[++i])
		free(node->argv[i]);
	free(node->argv);
	node->argv = tmp2;
}
