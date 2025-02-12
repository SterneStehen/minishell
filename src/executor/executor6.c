/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:34:14 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:15:56 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Checks for incorrect redirection in command.
	* This function verifies if there are any invalid redirections 
	in the given command list.
	* It ensures that each redirection token is followed by a 
	valid token and not another redirection. */
int	check_invalid_redirection(t_simple_cmds *command)
{
	t_simple_cmds	*cr;
	t_node			*tkn;

	cr = command;
	while (cr != NULL)
	{
		tkn = cr->redirections;
		while (tkn != NULL)
		{
			if (!tkn || !(tkn->class == LESS_LESS || tkn->class == GREAT_GREAT
					|| tkn->class == LESS || tkn->class == GREAT)
				|| tkn->next == NULL || (tkn->next->class == LESS_LESS
					|| tkn->next->class == GREAT_GREAT
					|| tkn->next->class == LESS || tkn->next->class == GREAT))
			{
				return (1);
			}
			tkn = tkn->next->next;
		}
		cr = cr->next;
	}
	return (0);
}

/* Runs here documents (heredocs) in command list.
 * This function processes heredoc redirections in the command list.
 * It replaces the heredoc delimiters with their corresponding content. */
void	run_heredoc_commands(t_simple_cmds *start, t_simple_cmds *last,
		t_tools *sh)
{
	t_node	*token;
	char	*tmp_str;

	while (start != last->next)
	{
		token = start->redirections;
		while (token != NULL)
		{
			if (is_redirection_token(token)
				&& is_redirection_token(token->next))
				return ;
			if (token->class == 'H')
			{
				tmp_str = get_break_string(token->next->data, sh);
				free(token->next->data);
				token->next->data = tmp_str;
				start->hd_file_name = generate_temp_heredoc(start,
						token->next->data, sh);
			}
			token = token->next;
		}
		start = start->next;
	}
}

/* Checks for invalid redirections in commands.
 * This function checks each command in the list for invalid redirections.
 * If an invalid redirection is found,
	it processes heredoc commands and returns 1. */
int	check_c__invalid_redir(t_simple_cmds *tbl, t_tools *sh)
{
	t_simple_cmds	*st;
	t_simple_cmds	*lst;

	if (check_invalid_redirection(tbl))
	{
		st = tbl;
		lst = tbl;
		while (lst != NULL)
		{
			if (check_invalid_redirection(lst))
			{
				run_heredoc_commands(st, lst, sh);
				return (1);
			}
			lst = lst->next;
		}
	}
	return (0);
}

/* Executes commands in a pipeline.
 * This function runs each command in the pipeline,
	handling the necessary process forking and piping.
 * It waits for all child processes to complete before returning. */
void	execute_pipeline_commands(t_simple_cmds *table, t_tools *shell)
{
	int	status;
	int	pid;

	while (table->next != NULL)
	{
		run_pipeline_child(table, shell);
		table = table->next;
	}
	finalize_pipeline_execution(table, shell);
	pid = waitpid(0, &status, 0);
	while (pid != -1)
		pid = waitpid(0, &status, 0);
}

/* Creates a pipe and handles errors.
	* This function initializes a pipe and exits with an 
	error message if the pipe creation fails. */
int	initialize_pipe(int *fd, t_tools *shell)
{
	if (pipe(fd) == -1)
	{
		error_exit(shell, "pipe failed", 0);
		return (0);
	}
	return (1);
}
