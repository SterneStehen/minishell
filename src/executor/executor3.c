/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:34:14 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:13:58 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Exits the program with an error message.
	* This function prints an error message and e
	xits the program with the given status. */
void	error_exit(t_tools *shell, const char *msg, int exit_st)
{
	if (shell->flag_log == 1)
	{
		printf("%s: %s:  %s\n", "minishell: ", shell->message, msg);
		shell->flag_log = 0;
	}
	if (exit_st != 2)
		exit(shell->last_status);
}

/* Handles directory errors in command execution.
 * This function handles errors related to directory commands,
 * printing an error message and exiting with a specific status code. */
void	handle_directory_command(char *cmd, t_tools *shell)
{
	if (shell->flag_log == 1)
	{
		printf("%s%s: %s\n", MINI, cmd, ISDIR);
		shell->flag_log = 0;
	}
	shell->last_status = 126;
	cleanup_child_proc(shell);
	exit(shell->last_status);
}

/* Executes the final command in a command table.
 * This function handles the execution of the final command in a command table,
 * managing redirections and ensuring proper cleanup. */
void	execute_final_command(char *command_path, t_simple_cmds *table,
		t_tools *shell)
{
	char	**cmd_args_copy;
	char	**env_copy;
	int		exec_result;

	shell->flag_execution_completed = 1;
	cmd_args_copy = arr_dabl(table->arguments);
	env_copy = arr_dabl(shell->envp);
	exec_result = execve(command_path, cmd_args_copy, env_copy);
	if (exec_result == -1)
	{
		if (shell->commands)
			error_exit(shell, strerror(errno), 0);
		free(command_path);
		free_string_array(cmd_args_copy);
		free_string_array(env_copy);
		exit(exec_result);
	}
}

/* Finalizes the execution of a command.
 * This function handles the final steps of executing a command,
 * including managing directory commands and finalizing the execution. */
int	finalize_command_path(char *command_path, t_simple_cmds *table,
		t_tools *shell)
{
	int	cmd_length;

	cmd_length = strlen(table->command);
	if (table->command[0] == '/' && table->command[cmd_length - 1] == '/')
	{
		handle_directory_command(table->command, shell);
	}
	execute_final_command(command_path, table, shell);
	return (0);
}

/* This function checks for specific exit conditions for a 
command and exits with appropriate status codes. */

int	check_strings(char *str1, char *str2)
{
	int	i;

	i = 0;
	if (strlen(str1) != strlen(str2))
		return (0);
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] != str2[i])
			return (0);
		i++;
	}
	return (1);
}
