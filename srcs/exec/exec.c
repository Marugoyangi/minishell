#include "minishell.h"

// 실행부 시작
void    exec_ast(t_node *root)
{
    t_node *cur;

    cur = root;
    whiel (cur)
    {
        if (cur->type == L_SUBSHELL)
            exec_subshell(cur);
        else if (cur->type == L_REDIRECTION)
            exec_redirection(cur);
        else if (cur->type == L_SIMPLE_COMMAND)
            exec_command(cur);
        else if (cur->type == L_PIPELINE)
            exec_pipeline(cur);
        else if (cur->type == L_LOGICAL_OPERATOR)
            exec_logical_operator(cur);
        // cur = cur->right;
    }
}

void    exec_pipeline(t_node *node)
{
    t_node  *cur_command;
    int pipe[2];

    cur_command = node;
    while (cur_command)
    {
        pipe(pipe);
        if (!pipe)
        {
            close(pipe[0]);
            dup2(pipe[1], 1);
            close(pipe[1]);
            exec_ast(cur_command);
            exit (0);
        }
        else
        {
            close(pipe[1]);
            dup2(pipe[0], 0);
            close(pipe[0]);
            cur_command = cur_command->right;
            waitpid(pid, NULL, 0);
        }
    }

}
void    exec_logical_operator(t_node *node)
{
    int status;
    pid_t   pid;
    pid = fork;

    if (!pid)
    {
        exec_ast(node->left);
        exit(0);
    }
    else
    {
        waitpid(pid, &status, 0);
        // 자식프로세스의 종료상대를 확인하기 위한 메크로
        // WIFEXITED, 자식 프로세스가 정상적으로 종료되었는지 확인 (정상 종료시 1)
        // WIFSTATUS, 자식 프로세스의 종료코드 (exit(?))를 추출, WIFEXITED가 참일때만 사용해야한다.
        // 시그널에 의한 종료인지, 정상적인 exit(0) 종료인지 확인을 위해 WIFSTATUS가 필요하다.
        if (node->data[0] == '&' && WIFEXITED(status) && !WEXITSTATUS(status))
            exec_ast(node->right);
        if (node->data[0] == '|' && !WIFEXITED(status) && WEXITSTATUS(status))
            exec_ast(node->right);
    }
}

// 서브쉘
void    exec_subshell(t_node *node)
{
    pid_t pid;

    pid = fork();
    
    if (pid == -1)
    {
        printf("error fork\n");
        exit (1);
    }
    else if (pid == 0)
    {
        execute_ast(node->left);
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
}

void    exec_command(t_node *node)
{
    // data의 내장기능함수와 인수 구분처리
    execute_builtin();
}

