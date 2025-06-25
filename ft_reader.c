#include "minishell.h"

void    exec_cmds(char *full_path, char **args, char **env)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        execve(full_path, args, env);
        perror("execv");
        exit(1);
    }
    else
    {
        wait(NULL);
    }
    exit_code = 0;
    free(full_path);
}

void    ft_cmds(t_list *mini, t_cmd *cmd, char **env)
{
    int prev_pipe_in = -1;
    int pipe_fd[2];

    while (cmd)
    {
        if (cmd->next)
            pipe(pipe_fd);

        pid_t pid = fork();
        if (pid == 0)
        {
            if (cmd->redirections)
                apply_redirections(cmd->redirections);            
            if (prev_pipe_in != -1)
                dup2(prev_pipe_in, STDIN_FILENO);
            if (cmd->next)
                dup2(pipe_fd[1], STDOUT_FILENO);
            exec_command(cmd->command, mini->paths, env);
            exit(1);
        }
        if (prev_pipe_in != -1)
            close(prev_pipe_in);
        if (cmd->next)
        {
            close(pipe_fd[1]);
            prev_pipe_in = pipe_fd[0];
        }

        cmd = cmd->next;
    }
    while (wait(NULL) > 0);
}