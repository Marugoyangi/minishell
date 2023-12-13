/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_var_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbpa <jeongbpa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 04:15:15 by jeongbpa          #+#    #+#             */
/*   Updated: 2023/12/05 04:15:15 by jeongbpa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell_vars(t_arg *arg)
{
	char	*tmp;

	if (find_env(arg->envp_head, "SHLVL"))
	{
		tmp = ft_itoa(ft_atoi(find_env(arg->envp_head, "SHLVL")) + 1);
		set_env(arg->envp_head, "SHLVL", tmp);
	}
	else
		append_env(arg->envp_head, "SHLVL", "1");
	if (find_env(arg->envp_head, "HOME"))
		arg->tilde = ft_strdup(find_env(arg->envp_head, "HOME"));
	else if (find_env(arg->envp_head, "USER"))
		arg->tilde = ft_strjoin("/home/", find_env(arg->envp_head, "USER"));
	else
		arg->tilde = getcwd(NULL, 0);
	arg->last_exit_status = 0;
	//PS1은 없으면 minishell$로 설정 -> 쉘 리드라인마다 적용됨
	//SHELL 변수는 현재경로minishell로 설정	
	//SHLVL은 현재 프로세스 깊이로 설정 (내가 아무 숫자로 바꾸면 fork시에 거기서 +1됨 변경)
	// 값을 못찾으면 fork시에 1로 설정됨

	//PWD는 현재 경로로 설정 cd할때마다 바뀜 (PWD를 바꿔도 현재폴더는 안바뀜)
	
	//OLDPWD는 이전 경로로 설정 (이거 뭐 저장하는 이유 있는걸로 앎)

	//MINISHELL이랑 "_"는 현재경로 minishell로 설정/home/jungmk2/3_minishell/./minishell
	//PATH도 아마 필요함?
	//HOME은 홈경로
	//USER는 유저이름
	// 또 뭐 필요한 거 있나?... 고민해볼것 
}