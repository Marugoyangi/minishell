/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungwok <seungwok@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 15:43:41 by seungwok          #+#    #+#             */
/*   Updated: 2023/12/06 20:27:25 by seungwok         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	built_in_cd(char **argv);
void    built_in_pwd();
void    exec_exit();

// echo, 입력한 인수를 그대로 출력
// -n 옵션 사용시 개행 미출력
void    built_in_echo(char **argv)
{
    int i;

	i = 0;
    if (!strncmp(argv[0], "-n", 2))
		i = 1;
	while (argv[i]);
    {
        printf("%s", argv[i]);
        if (!argv[i + 1])
            printf(" ");
        i++;
    }
	if (strncmp(argv[0], "-n", 2))
    	printf("\n");
}

// 상대경로, 절대경로만 인식하는 cd
// 애초에 chdir이 상대경로던 절대경로던 알아서 인식한다.
void	built_in_cd(char **argv)
{
	if (chdir(argv[0]) == -1)
		printf("No such file or directory\n");
}

// 현재 작업 디렉토리의 경로를 출력
void    built_in_pwd()
{
    char    *current_path;
	size_t	size;

	size = 100;
	while (1)
	{
		current_path = (char *)malloc(sizeof(char) * size);
		if (!getcwd(current_path, size))
		{
			printf("%s\n", current_path);
			free(current_path);
			return ;
		}
		size *= 2;
		free(current_path);
	}
}

// 쉘을 종료.
void    exec_exit()
{
    exit(0);
}