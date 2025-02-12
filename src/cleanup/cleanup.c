/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:55:21 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 19:10:48 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* General purpose memory release function. */
void	free_resources(t_tools *shell)
{
	if (shell->shell_string)
		cleanup_command_tables(shell->commands);
	free_string_array(shell->paths);
	free(shell->shell_string);
	free_string_array(shell->envp);
	release_env_list(shell->envair);
	free(shell->content_hd);
	free(shell->message);
	rl_clear_history();
}

/* Free memory on exit. */
void	cleanup_before_exit(t_tools *shell)
{
	free_resources(shell);
}

/* Free memory in the child process. */
void	cleanup_child_proc(t_tools *shell)
{
	free_resources(shell);
}

/* Builtin release function to free memory and exit. */
void	release_builtin_resources(t_tools *shell)
{
	int	result;

	result = shell->last_status;
	if (shell->shell_string)
		cleanup_command_tables(shell->commands);
	free_string_array(shell->paths);
	free(shell->shell_string);
	free_string_array(shell->envp);
	release_env_list(shell->envair);
	free(shell->content_hd);
	free(shell->message);
	rl_clear_history();
	exit(result);
}

/* Exit the child process and clean up resources. */
void	child_process_finish(t_tools *shell)
{
	int	code;

	code = shell->last_status;
	if (shell->shell_string)
		cleanup_command_tables(shell->commands);
	free_string_array(shell->paths);
	free_string_array(shell->envp);
	release_env_list(shell->envair);
	free(shell->content_hd);
	free(shell->message);
	rl_clear_history();
	exit(code);
}
