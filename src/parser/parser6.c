/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:47:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:20:20 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Processes a command table by removing quotes from commands and arguments.
   Converts certain commands to lowercase. */
void	handle_command_table(t_simple_cmds *table, t_tools *tools)
{
	char	*temp;

	if (table->command != NULL)
	{
		temp = remove_quotes(table->command);
		free(table->command);
		table->command = temp;
		optimize_commands(table);
	}
	if (are_strings_equal(table->command, "echo") == 0
		&& are_strings_equal(table->command, "export") == 0)
		delit_quotes_command(table, tools);
}

/* Removes quotes from the command and arguments in a command table. */
void	delit_quotes_command(t_simple_cmds *table, t_tools *shell)
{
	(void)shell;
	manage_process_tokens(table->token_arg);
	manage_process_tokens(table->redirections);
}

/* Removes quotes from all command tables. */
void	delite_quotes_tables(t_simple_cmds *tables, t_tools *shell)
{
	while (tables != NULL)
	{
		handle_command_table(tables, shell);
		tables = tables->next;
	}
}

/* Calculates the number of tokens in a linked list of tokens.
   Returns the count of tokens. */
int	calculate_token_size(t_node *token)
{
	int		count;
	t_node	*current;

	count = 0;
	current = token;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/* Counts the number of tokens in a linked list of tokens.
   Returns the count of tokens. */
int	calculate_token_length(t_node *token)
{
	int	i;

	i = 0;
	if (token == NULL)
	{
		return (0);
	}
	else
	{
		i = calculate_token_size(token);
		return (i);
	}
}
