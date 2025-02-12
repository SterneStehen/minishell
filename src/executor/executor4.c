/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:34:14 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:27:50 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Executes the final command in a pipeline.
 * This function handles the execution of the final command in the pipeline,
 * managing any necessary redirections and final cleanup. */
int	finalize_pipeline_execution(t_simple_cmds *table, t_tools *shell)
{
	int	has_redir;

	has_redir = 0;
	spawn_and_execute(table, shell, &has_redir);
	if (table->command != NULL)
		execute_builtin_commands(shell, table->command, table->arguments);
	if (invalid_redirection(shell, table->redirections, table) == 0)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		dup2(shell->standard_input_fd, STDIN_FILENO);
		dup2(shell->standard_output_fd, STDOUT_FILENO);
		if (shell->last_status != 0)
			shell->flag_execution_completed = 0;
	}
	return (0);
}

/* Restores standard input and output file descriptors.
 * This function resets the standard input and output file descriptors
 * to their original values,
	ensuring that subsequent commands can execute correctly. */
int	reset_file_descriptors(t_tools *sh)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	dup2(sh->standard_input_fd, STDIN_FILENO);
	dup2(sh->standard_output_fd, STDOUT_FILENO);
	if (sh->last_status)
	{
		sh->flag_execution_completed = 0;
	}
	return (0);
}

/* Executes built-in commands and restores file descriptors.
 * This function handles the execution of built-in commands and ensures that
 * file descriptors are properly reset after execution. */
int	run_builtin_and_close(t_simple_cmds *cmd_table, t_tools *sh)
{
	if (cmd_table->command != NULL)
	{
		execute_builtin_commands(sh, cmd_table->command, cmd_table->arguments);
	}
	if (!invalid_redirection(sh, cmd_table->redirections, cmd_table))
	{
		reset_file_descriptors(sh);
	}
	else
		sh->last_status = REDIR_ERR_CODE;
	return (0);
}

/* Handles execution of a command table.
 * This function manages the execution of commands, including signal handling,
	redirections, and command execution. */
int	handle_execution(t_simple_cmds *cmd_table, t_tools *sh)
{
	char	**cmd_args_copy;
	char	**env_copy;
	int		exit_status;

	config_signals_child(&sh->terminal_setting);
	if (!invalid_redirection(sh, cmd_table->redirections, cmd_table))
	{
		sh->flag_log = 1;
		manage_redirections(sh, cmd_table);
		execute_command(cmd_table, sh);
	}
	exit_status = sh->last_status;
	cmd_args_copy = arr_dabl(cmd_table->arguments);
	env_copy = arr_dabl(sh->envp);
	if (execve(cmd_table->command, cmd_args_copy, env_copy) == -1)
	{
		free_string_array(cmd_args_copy);
		free_string_array(env_copy);
		return (exit_status);
	}
	return (exit_status);
}

/* Executes a single command.
 * This function handles the execution of a single command, managing forking,
	redirections, and cleanup. */
int	simple_execute(t_simple_cmds *commands, t_tools *tools)
{
	pid_t	child_pid;
	int		wait_status;

	tools->flag_pipe = 0;
	tools->flag_ready_to_execute = 1;
	child_pid = fork();
	if (child_pid == 0)
	{
		handle_execution(commands, tools);
		child_process_finish(tools);
	}
	else if (child_pid == -1)
	{
		error_exit(tools, FORK_ERR_MSG, 0);
		return (-1);
	}
	else
	{
		configure_signals_parent();
		tools->flag_log = 0;
		waitpid(child_pid, &wait_status, 0);
		tools->last_status = WEXITSTATUS(wait_status);
		run_builtin_and_close(commands, tools);
	}
	return (0);
}
