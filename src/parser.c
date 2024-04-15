#include "../inc/minishell.h"

int parse_input(char *input, t_command **commands) 
{
    if (input == NULL) 
	{
        *commands = NULL;
        return 0;
    }

    char **command_strings = ft_split(input, ';');
    if (command_strings == NULL) 
	{
        *commands = NULL;
        return 0;
    }

    int num_commands = 0;
    while (command_strings[num_commands] != NULL) 
	{
        num_commands++;
    }

    *commands = malloc(sizeof(t_command) * num_commands);
    if (*commands == NULL) 
	{
        ft_free_array(command_strings);
        return 0;
    }

    int executed_commands = 0;
    for (int i = 0; i < num_commands; i++)
	{
        char *trimmed_command = ft_strtrim(command_strings[i], " ");
        if (trimmed_command == NULL) 
		{
            continue;
        }
        char **args = ft_split(trimmed_command, ' ');
        free(trimmed_command);
        if (args == NULL)
		{
            continue;
        }

        t_command command;
        command.cmd = ft_strdup(args[0]);
        command.args = args;

        execute_command(&command);

        free(command.cmd);
        ft_free_array(args);

        executed_commands++;
    }

    ft_free_array(command_strings);
    free(*commands);
    *commands = NULL;
    return (executed_commands);
}
