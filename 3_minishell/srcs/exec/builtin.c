#include "minishell.h"

// 빌트인 함수들
void    exec_builtin(char *cmd, char *arg)
{
    int i;

    i = 0;
    // 빌트인 분기점
}

// echo, 입력한 인수를 그대로 출력
void    exec_echo(char **arg)
{
    int i;

    i = 0
    while (arg[i])
    {
        printf("%s", arg[i]);
        if (!arg[i + 1])
            printf(" ");
        i++;
    }
    printf("\n")
}

// 현재 작업 디렉토리를 변경, 사용자가 입력한 디렉토리로 이동
void    exec_cd(char **arg)
{
    if (chdir(arg[1]) == -1)
        printf("error cd\n");
}

// 현재 작업 디렉토리의 경로를 출력
void    exec_pwd()
{
    char    current_path[10000];

    if (getcwd(current_path, sizeof(current_path))!)
        printf("%s\n", current_path);
    else
        printf("error pwd\n");
}

// 쉘을 종료.
void    exec_exit(char **arg)
{
    exit(0);
}