#include "minishell.h"

// 환경 변수 설정
void    exec_export(char **arg)

// 환경 변수 제거
void    exec_unset(char **arg)

// 현재 시스템의 환경 변수들을 출력
void    exec_env()
{
    extern char **environ;

    char **env = environ;
    while (!*env)
    {
        printf("%s\n", *env);
        env++;
    }
}