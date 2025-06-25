#include "minishell.h"

void paths(char **env, t_list *mini)
{
    int i = 0;
    while (env[i])
    {
        if (ft_strncmp("PATH=", env[i], 5) == 0)
            mini->path = strdup(env[i] + 5);
        if (ft_strncmp("HOME=", env[i], 5) == 0)
            mini->home = strdup(env[i] + 5);
        i++;
    }
    mini->paths = ft_split(mini->path, ':');
}

void handle_sig(int signum)
{
    (void)signum;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

int node_c(t_cmd *node)
{
    int i;
    i = 0;
    t_cmd *tmp;
    tmp = node;
    while(node)
    {
        i++;
        tmp = tmp->next;
    }
    return(i);
}

void exec_command(char **args, char **paths, char **env)
{
    char *full_path = get_full_path(args[0], paths);
    if (!full_path)
    {
        fprintf(stderr, "minishell: %s: command not found\n", args[0]);
        exit(127);
    }
    execve(full_path, args, env);
    perror("execve");
    exit(1);
}

char *get_full_path(char *cmd, char **paths)
{
    int i;
    i = 0;
    if (strchr(cmd, '/'))
        return strdup(cmd);

    while (paths[i])
    {
        char *full_path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(full_path, cmd);
        if (access(full_path, X_OK) == 0)
            return full_path;
        free(full_path);
        i++;
    }
    return NULL;
}
