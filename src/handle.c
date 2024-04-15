#include "../inc/minishell.h"

void handle_cd(t_command *command)
{
    if (command->args[1] != NULL) 
    {
        if (chdir(command->args[1]) != 0) 
        {
            perror("minishell: cd");
        }
    } 
    else 
    {
        fprintf(stderr, "minishell: cd: missing argument\n");
    }
}

void handle_echo(t_command *command)
{
    int i = 1;
    bool n_flag = false;

    if (command->args[i] && strcmp(command->args[i], "-n") == 0)
    {
        n_flag = true;
        i++;
    }
    
    while (command->args[i]) 
    {
        printf("%s", command->args[i]);
        if (command->args[++i]) 
        {
            printf(" ");
        }
    }

    if (!n_flag) 
    {
        printf("\n");
    }
}

void handle_pwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        printf("%s\n", cwd);
    } 
    else 
    {
        perror("minishell: pwd");
    }
}

void handle_env()
{
    char **env = environ;
    while (*env) 
    {
        printf("%s\n", *env);
        env++;
    }
}

void print_history()
{
    HIST_ENTRY **the_history = history_list();
    if (the_history)
	{
        for (int i = 0; the_history[i]; i++)
		{
            printf("%d: %s\n", i + 1, the_history[i]->line);
        }
    }
}

void free_exit(t_command *command) 
{
    if (command == NULL) 
	{
        return;
    }
    free(command->cmd);
    if (command->args != NULL)
	{
        int i = 0;
        while (command->args[i] != NULL) 
		{
            free(command->args[i]);
            i++;
        }
        free(command->args);
    }
	exit(0);
}
