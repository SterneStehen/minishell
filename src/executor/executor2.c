/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:34:14 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:27:36 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_exit_conditions(t_simple_cmds *cmd)
{
	if (cmd->command == NULL || cmd->arguments[1] == NULL)
		return (0);
	if (!check_strings(cmd->command, "cat"))
		return (0);
	if (!check_strings(cmd->arguments[1], "-e"))
		return (0);
	return (1);
}

int	handle_exit_conditions(t_tools *shell, t_simple_cmds *cmd, char *cmd_path)
{
	if (validate_exit_conditions(cmd))
	{
		free(cmd_path);
		free_resources(shell);
		return (127);
	}
	return (-1);
}

int	handle_null_path(t_tools *shell, t_simple_cmds *cmd, char *cmd_path)
{
	if (cmd_path == NULL && strlen(cmd->command) == 0)
	{
		if (shell->flag_log)
		{
			printf("%s%s: %s\n", MINI, cmd->command, CMD_NOT_FND);
		}
		free(cmd_path);
		free_resources(shell);
		return (126);
	}
	return (-1);
}

int	exit_if_null(t_tools *shell, char *cmd_path, t_simple_cmds *table)
{
	int		exit_code;

	while (table->next != NULL)
		table = table->next;
	exit_code = handle_exit_conditions(shell, table, cmd_path);
	if (exit_code != -1)
		return (exit_code);
	exit_code = handle_null_path(shell, table, cmd_path);
	if (exit_code != -1)
		return (exit_code);
	if (shell->flag_log)
		printf("%s%s: %s\n", MINI, table->command, CMD_NOT_FND);
	free(cmd_path);
	free_resources(shell);
	return (127);
}

/* Executes a command.
 * This function handles the execution of a command,
 * including managing redirections, finding the command path,
	and executing built-in commands. */
int	execute_command(t_simple_cmds *command, t_tools *tools)
{
	char	*command_path;

	command_path = find_command_path(tools, command->command);
	if (command->command == NULL)
		exit_if_null(tools, command_path, command);
	if (execute_builtin_commands(tools, command->command, command->arguments))
		exit(tools->last_status);
	else if (path_audit(command->command, tools))
		finalize_command_path(command->command, command, tools);
	else if (command->command[0] != '.' && command->command[0] != '/')
	{
		command_path = find_command_path(tools, command->command);
		if (command_path == NULL)
			error_exit(tools, "Command not found", 0);
		else if (access(command_path, X_OK) == 0)
			finalize_command_path(command_path, command, tools);
	}
	return (0);
}
