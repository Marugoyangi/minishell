// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   exec_redirection_utils.c                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: woopinbell <woopinbell@student.42.fr>      +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/12/12 12:15:31 by seungwok          #+#    #+#             */
// /*   Updated: 2023/12/12 22:29:50 by woopinbell       ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../minishell.h"

// int	check_built_in_redirection(t_node *node);
// int external_command_redirection(t_node *node, t_arg *arg, int fd, int fd_sign);
// int	external_command_redirection_child(t_node *node, t_arg *arg, int fd, int fd_sign);


// int	check_built_in_redirection(t_node *node)
// {
// 	if (!ft_strcmp(node->data, "echo") 
// 		|| !ft_strcmp(node->data, "cd")
// 		|| !ft_strcmp(node->data, "pwd")
// 		|| !ft_strcmp(node->data, "exit")
// 		|| !ft_strcmp(node->data, "export")
// 		|| !ft_strcmp(node->data, "unset")
// 		|| !ft_strcmp(node->data, "env"))
// 		return (0);
// 	return (1);
// }

// int external_command_redirection(t_node *node, t_arg *arg, int fd, int fd_sign)
// {
// 	int		status;
// 	pid_t	pid;

// 	status = 0;
// 	pid = fork();
// 	if (!pid)
// 		external_command_redirection_child(node, arg, fd, fd_sign);
// 	else
// 		waitpid(pid, &status, 0);	// 부모 프로세서는 status에 자식프로세스의 종료상태를 담아서 반환.
// 	return (status);
// }

// int	external_command_redirection_child(t_node *node, t_arg *arg, int fd, int fd_sign)
// {
// 	char	**path;
// 	char	*excutable_path;

// 	path = set_path(arg->envp_head);
// 	dup2(fd, fd_sign);
// 	close(fd);

// 	// if (!ft_strncmp(node->data, "./", 2) || !ft_strncmp(node->data, "../", 3 || !ft_strncmp(node->data, "/", 1))) 파일명만 들어온경우 path에서만 검색하도록 수정 필요.
// 	if (!(execve(node->argv[0], node->argv, arg->envp) == -1))
// 		exit(0);
// 	else
// 	{
// 		excutable_path = find_path(path, node->data);
// 		if (!excutable_path)
// 			exit (1);
// 		if (execve(excutable_path, node->argv, arg->envp) == -1)	// execve함수를 통한 새로운 프로세서 생성 실패시 perror 에러출력
// 		{
// 			perror("execve");
// 			exit (1);
// 		}
// 		exit (0);
// 	}
// }
