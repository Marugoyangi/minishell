#include "minishell.h"

// 리다이렉션
void    exec_redirection(t_node *node)
{
    if (!strcmp(node->data, ">"))
        exec_output(node);
    else if (!strcmp(node->data, ">>"))
        exec_append(node);
    else if (!strcmp(node->data, "<"))
        exec_input(node);
    else
        exec_heredoc(node);
}

void    exec_output(t_node *node)
{
    int fd;
    
    // O_TRUNC 이미 파일이 존재하는경우 해당 파일을 비우기때문에 덮어쓰기 효과.
    fd = open(node->right->data, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
    {
        printf("error open\n");
        exit (1);
    }
    dup2(fd, 1);
    close(fd);
    exec_ast(node->left);   // 표준 출력으로 복제 후 노드 내용 실행
}

void    exec_append(t_node *node)
{
    int fd;

    // O_APPEND 파일이 존재하는 경우 파일 끝에 내용을 추가하기때문에 이어쓰기 효과.
    fd = open(node->right->data, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1)
    {
        printf("error open\n");
        exit (1);
    }
    dup2(fd, 1);
    close(fd);
    exec_ast(node->left);   // 표준 출력으로 복제 후 노드 내용 실행
}

void    exec_input(t_node *node)
{
    int fd;

    fd = open(node->right->data, O_RDONLY);
    if (fd == -1)
    {
        printf("error open\n");
        exit (1);
    }
    dup2(fd, 0);
    close(fd);
    exec_ast(node->left);   // 표준 입력으로 복제 후 노드 내용 실행
}

void    exec_heredoc(t_node *node)
{
    int     pipe[2];
    pid_t   pid;

    // pipe[1]에 쓴 내용을 pipe[0]에서 읽는다.
    // 함수의 줄수제한때문에, fork와 pipe함수의 에러에대한 처리를해야하는가?
    // 해야한다면 따로 함수를 만들어서 하는게 나을거같음.
    if (pipe(pipe) == -1)
    {
        printf("error pipe\n");
        exit (1);
    }
    pid = fork();
    if (pid == -1)
    {
        printf("error fork\n");
        exit (1);
    }
    else if (pid == 0)
    {
        close(pipe[0]);
        write(pipe[1], node->right->data, ft_strlen(node->right->data));
        write(pipe[1], "\n", 1);
        close(pipe[1]);
        exit(0);
    }
    else
    {
        close(pipe[1]);
        dup2(fd[0], 0); //표준입력으로 복제
        close(pipe[0]);
        exec_ast(node->left);
        waitpid(pid, NULL, 0);
    }
}

