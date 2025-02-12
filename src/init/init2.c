/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:43:15 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:34:37 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Sets redirection tokens in the command table.
   Clones and appends redirection tokens to the redirections list. */
t_node	*install_redirection(t_simple_cmds *cmd_tbl, t_node *token)
{
	t_node	*new_token;

	new_token = NULL;
	if (token == NULL)
		return (token);
	if (is_redirection_token(token) == TRUE
		|| is_redirection_token(token->prev) == TRUE)
	{
		return (process_redirection(cmd_tbl, token, new_token));
	}
	return (token);
}

/* Clones the given token and appends it to the command table's token_arg list.
   Returns the next token in the list. */
t_node	*handle_new_token(t_simple_cmds *cmd_table, t_node *tok)
{
	t_node	*new_tok;

	new_tok = copy_list(tok);
	cmd_table->token_arg = append_token(cmd_table->token_arg, new_tok);
	return (tok->next);
}

/* Validates the given token for setting as a command argument.
   Returns 1 if valid, otherwise returns 0. */
int	check_token_validity(t_simple_cmds *cmd_table, t_node *tok)
{
	if (tok == NULL)
		return (0);
	if (tok->class != STRING)
		return (0);
	if (is_redirection_token(tok->prev) == TRUE)
		return (0);
	if (cmd_table->command == NULL)
		return (0);
	return (1);
}

/* Sets the given token as a command argument in the command table.
   Returns the next token in the list. */
t_node	*set_arg_to_command(t_simple_cmds *cmd_tbl, t_node *token)
{
	if (!check_token_validity(cmd_tbl, token))
	{
		return (token);
	}
	return (handle_new_token(cmd_tbl, token));
}

/* Appends a new command table to the end of the command list.
   Sets the count of the new command table. */
t_simple_cmds	*ext_comm_list(t_simple_cmds *comm,
		t_simple_cmds *new_command)
{
	t_simple_cmds	*current;

	if (comm == NULL)
	{
		new_command->count = 0;
		return (new_command);
	}
	current = comm;
	while (current->next != NULL)
		current = current->next;
	current->next = new_command;
	new_command->count = current->count + 1;
	return (comm);
}
