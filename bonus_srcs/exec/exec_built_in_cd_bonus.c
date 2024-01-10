/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in_cd_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:50:29 by jeongbpa          #+#    #+#             */
/*   Updated: 2024/01/10 11:26:59 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int		built_in_cd(t_node *node, t_arg *arg, char **argv);
int		built_in_cd_oldpwd(t_node *node, t_arg *arg);
void	built_in_cd_set_pwd_arg(char *tmp, t_arg *arg, t_node *node);
void	built_in_cd_set_pwd_env(t_node *node, t_arg *arg);

int	built_in_cd(t_node *node, t_arg *arg, char **argv)
{
	t_env	*cur;

	if (!argv[1])
		return (0);
	if (argv[2])
		return (exec_perror("cd: too many arguments", 0));
	if (!ft_strcmp(argv[1], "-"))
		return (built_in_cd_oldpwd(node, arg));
	chdir(argv[1]);
	if (arg->oldpwd)
		free(arg->oldpwd);
	arg->oldpwd = ft_strdup(arg->pwd);
	cur = find_env_node(arg->envp_head, "OLDPWD");
	if (cur)
	{
		if (cur->value)
			free(cur->value);
		cur->value = ft_strdup(arg->oldpwd);
	}
	built_in_cd_set_pwd_env(node, arg);
	return (0);
}

int	built_in_cd_oldpwd(t_node *node, t_arg *arg)
{
	t_env	*cur;
	char	*tmp;

	cur = find_env_node(arg->envp_head, "OLDPWD");
	if (!cur || !cur->value)
		return (exec_perror("cd: OLDPWD not set", 0));
	tmp = getcwd(0, 0);
	if (!tmp)
		return (exec_perror("cd", 0));
	if (chdir(arg->oldpwd) == -1)
	{
		free(tmp);
		return (exec_perror("cd", 0));
	}
	free(cur->value);
	free(arg->oldpwd);
	cur->value = tmp;
	arg->oldpwd = ft_strdup(tmp);
	built_in_cd_set_pwd_env(node, arg);
	return (0);
}

void	built_in_cd_set_pwd_arg(char *tmp, t_arg *arg, t_node *node)
{
	char	*str;

	if (!tmp)
	{
		str = ft_strdup("cd: error retrieving current directory: getcwd: ");
		str = modified_strjoin(str, "cannot access parent directories", 1);
		exec_perror(str, 0);
		free (str);
		arg->pwd = modified_strjoin(arg->pwd, "/", 1);
		arg->pwd = modified_strjoin(arg->pwd, node->argv[1], 1);
	}
	else
	{
		free(arg->pwd);
		arg->pwd = tmp;
	}
}

void	built_in_cd_set_pwd_env(t_node *node, t_arg *arg)
{
	char	*tmp;
	t_env	*cur;

	cur = find_env_node(arg->envp_head, "PWD");
	if (cur)
	{
		if (cur->value)
			free(cur->value);
		cur->value = getcwd(0, 0);
	}
	tmp = getcwd(0, 0);
	built_in_cd_set_pwd_arg(tmp, arg, node);
	if (!cur->value)
		cur->value = ft_strdup(arg->pwd);
}
