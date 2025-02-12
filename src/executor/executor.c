/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/05/23 21:19:12 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/23 21:19:12 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Handles redirections in commands.
 * This function checks and handles redirections in the given command table,
 * ensuring that invalid redirections are properly managed. */
void	handle_redirect(t_tools *sh, t_simple_cmds *tbl)
{
	if (check_c__invalid_redir(tbl, sh))
	{
		check_redir(sh->shell_string);
		sh->last_status = 258;
		cleanup_command_tables(sh->commands);
		sh->commands = NULL;
	}
}

/* Processes and runs heredocs in command tables.
 * This function processes and executes heredoc commands in the command table,
 * replacing heredoc delimiters with their corresponding content. */
void	process_heredocs_run(t_tools *sh, t_simple_cmds *tbl)
{
	t_simple_cmds	*current_tbl;
	t_node			*token;

	current_tbl = tbl;
	while (current_tbl != NULL)
	{
		token = current_tbl->redirections;
		while (token != NULL)
		{
			if (token->class == LESS_LESS)
			{
				execute_heredocs(current_tbl, sh);
				break ;
			}
			token = token->next;
		}
		current_tbl = current_tbl->next;
	}
}

/* Performs the execution of command tables.
 * This function executes the given command tables,
	managing pipelines and simple commands as needed. */
void	run_comm_exec(t_tools *tools, t_simple_cmds *commands)
{
	int				table_size;
	t_simple_cmds	*temp_tbl;

	g_global_flag = 0;
	table_size = 0;
	temp_tbl = commands;
	while (temp_tbl != NULL)
	{
		table_size++;
		temp_tbl = temp_tbl->next;
	}
	if (table_size == 1)
	{
		simple_execute(commands, tools);
	}
	else
	{
		tools->flag_pipe = 1;
		execute_pipeline_commands(commands, tools);
	}
}

/* Cleans up the shell structure.
 * This function releases resources associated with the shell structure,
 * ensuring proper cleanup after command execution. */
void	cleanup(t_tools *tools)
{
	cleanup_command_tables(tools->commands);
	tools->commands = NULL;
}

/* Main function to execute commands.
 * This function handles the overall execution flow of the shell,
 * managing redirections, heredocs, and command execution. */
void	run_exec_steps(t_tools *sh, t_simple_cmds *tbl)
{
	handle_redirect(sh, tbl);
	if (sh->commands == NULL)
		return ;
	process_heredocs_run(sh, tbl);
	run_comm_exec(sh, tbl);
	cleanup(sh);
}
