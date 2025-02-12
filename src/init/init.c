/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/05/30 22:52:41 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/30 22:52:41 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Validates the given token for setting as a command.
   Returns 1 if valid, otherwise returns 0. */
int	validate_command(t_simple_cmds *cmd_table, t_node *tok)
{
	if (tok == NULL)
		return (0);
	if (cmd_table->command != NULL)
		return (0);
	if (tok->class != STRING)
		return (0);
	if (is_redirection_token(tok->prev) == TRUE)
		return (0);
	return (1);
}

/* Sets the given token as the command in the command table.
   Returns the next token in the list. */
t_node	*set_command(t_simple_cmds *cmd_table, t_node *tok)
{
	cmd_table->command = ft_strdup(tok->data);
	return (tok->next);
}

/* Sets commands and arguments in the command table.
   Validates and processes tokens to set as commands and arguments. */
t_node	*set_commands(t_simple_cmds *cmd_tbl, t_node *token)
{
	if (!validate_command(cmd_tbl, token))
	{
		return (token);
	}
	return (set_command(cmd_tbl, token));
}

/* Processes tokens to set commands, arguments,
		and redirections in the command table.
   Iterates through tokens and sets them accordingly. */
t_node	*process_tokens_run(t_simple_cmds *cmd, t_node *tokens)
{
	t_node	*current_token;

	current_token = tokens;
	while (current_token != NULL)
	{
		current_token = set_commands(cmd, current_token);
		current_token = set_arg_to_command(cmd, current_token);
		current_token = install_redirection(cmd, current_token);
	}
	return (current_token);
}

/* Creates a new command table from the token list and appends it to the command
   tables list.
   Processes tokens and sets commands, arguments, and redirections. */
t_simple_cmds	*create_command_table(t_simple_cmds *command_tables,
		t_node *token_list)
{
	t_simple_cmds	*new_command;

	if (token_list == NULL)
		return (command_tables);
	new_command = create_empty_structure();
	if (new_command == NULL)
		return (command_tables);
	process_tokens_run(new_command, token_list);
	command_tables = ext_comm_list(command_tables, new_command);
	return (command_tables);
}
