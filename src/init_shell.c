#include "../inc/minishell.h"

int count_envp(char **envp)
{
    int count = 0;
    while (envp[count] != NULL) 
	{
        count++;
    }
    return count;
}

t_command *init_command(char **envp) 
{
    int env_count = count_envp(envp);
    t_command *command = malloc(sizeof(t_command));
    if (!command) {
        perror("Error memory");
        exit(EXIT_FAILURE);
    }

    command->cmd = NULL;
    command->args = NULL;

    command->envp = malloc((env_count + 1) * sizeof(char *));
    if (!command->envp) {
        perror("Error memory");
        free(command);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < env_count; i++) {
        command->envp[i] = strdup(envp[i]);
        if (!command->envp[i]) {
            perror("Error memory");
            while (i--) free(command->envp[i]);
            free(command->envp);
            free(command);
            exit(EXIT_FAILURE);
        }
    }
    command->envp[env_count] = NULL;

    return command;
}
