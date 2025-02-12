/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:34:14 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:27:55 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

	/* This function forks the current process and sets up the child process 
	for command execution,
 * including signal handling and file descriptor redirection. */
int	create_child_process(t_tools *shell, int *fd)
{
	pid_t	pid;

	pid = fork();
	shell->flag_ready_to_execute = 1;
	if (pid <= -1)
	{
		error_exit(shell, FORK_ERROR, 0);
		return (0);
	}
	else if (pid == 0)
	{
		config_signals_child(&shell->terminal_setting);
		shell->flag_log = 1;
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		return (1);
	}
	return (2);
}

/* Executes commands in the parent process.
 * This function handles the execution of commands in the parent process,
 * ensuring proper signal handling and file descriptor redirection. */
void	execute_parent_process(t_simple_cmds *table, t_tools *shell, int *fd)
{
	shell->flag_log = 0;
	if (table->command != NULL)
		execute_builtin_commands(shell, table->command, table->arguments);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

/* Handles child process execution for a pipeline.
 * This function sets up the pipeline, forks the process,
	and manages redirections and command execution
 * for each child process in the pipeline. */
int	run_pipeline_child(t_simple_cmds *table, t_tools *shell)
{
	int		fd[2];
	int		exec_flag;
	int		fork_result;

	exec_flag = initialize_pipe(fd, shell);
	if (exec_flag)
	{
		fork_result = create_child_process(shell, fd);
		if (fork_result == 1)
		{
			manage_redirections(shell, table);
			execute_command(table, shell);
			exit(0);
		}
		else if (fork_result == 0)
		{
			return (1);
		}
	}
	execute_parent_process(table, shell, fd);
	return (0);
}

/* Checks if a command has redirections.

	* This function checks the command's redirections 
	list to determine if it contains any output redirection tokens. */
int	validate_redirections(t_simple_cmds *table)
{
	t_node	*token;

	token = table->redirections;
	while (token != NULL)
	{
		if (token->class == GREAT_GREAT || token->class == GREAT)
		{
			return (1);
		}
		token = token->next;
	}
	return (0);
}

/* Forks and executes a command, handling redirections.
 * This function forks the process and executes the command,
 * setting up necessary redirections and managing the child process. */
void	spawn_and_execute(t_simple_cmds *table, t_tools *shell, int *has_redir)
{
	pid_t	pid;
	int		status;

	pid = fork();
	shell->flag_ready_to_execute = 1;
	if (pid == -1)
	{
		error_exit(shell, FORK_ERROR, 0);
		return ;
	}
	else if (pid == 0)
	{
		config_signals_child(&shell->terminal_setting);
		shell->flag_log = 1;
		*has_redir = validate_redirections(table);
		if (*has_redir == 0)
			dup2(shell->standard_output_fd, STDOUT_FILENO);
		manage_redirections(shell, table);
		execute_command(table, shell);
		exit(0);
	}
	configure_signals_parent();
	shell->flag_log = 0;
	waitpid(pid, &status, 0);
	shell->last_status = WEXITSTATUS(status);
}
