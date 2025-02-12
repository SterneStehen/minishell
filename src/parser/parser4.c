/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:47:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:34:14 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Processes a quoted substring and adds it as a token.
   Returns the updated token list. */
t_node	*add_quote(char *input_str, int *current_index, int *previous_index,
		t_node *token)
{
	char	quote_char;
	int		stop_search;

	if (input_str[*current_index] == '\0')
		return (token);
	if (input_str[*current_index] != '\''
		&& input_str[*current_index] != '\"')
		return (token);
	quote_char = input_str[(*current_index)++];
	stop_search = 0;
	while (input_str[*current_index] != '\0' && stop_search == 0)
	{
		if (input_str[*current_index] == quote_char
			&& calcul_escaped_charact(input_str, *current_index) % 2 == 0)
			stop_search = 1;
		(*current_index)++;
	}
	if (*current_index - 1 != *previous_index)
		token = insert_token(token, duplicate_string_range(input_str,
					*previous_index, *current_index), STRING);
	*previous_index = *current_index;
	return (token);
}

/* Extracts command arguments from the command and token list.
   Returns an array of command arguments. */
char	**extract_cmd_args(char *cmd, t_node *token)
{
	int		size;
	char	**cmd_args;
	int		index;

	cmd_args = allocate_cmd_args(cmd, token, &size);
	if (cmd_args == NULL)
		return (NULL);
	index = 0;
	if (cmd != NULL)
	{
		cmd_args[index++] = strdup(cmd);
	}
	while (token != NULL && index < size)
	{
		cmd_args[index++] = strdup(token->data);
		token = token->next;
	}
	return (cmd_args);
}

/* Initializes command arguments for all command tables. */
void	init_commands(t_simple_cmds *tables)
{
	while (tables != NULL)
	{
		tables->arguments = extract_cmd_args(tables->command,
				tables->token_arg);
		tables = tables->next;
	}
}

/* Splits a string into tokens.
   Returns the head of the token list. */
t_node	*tokenize_slip_string(char *str, t_node *token)
{
	int	current_index;
	int	previous_index;

	current_index = 0;
	while (str[current_index] != '\0')
	{
		previous_index = current_index;
		token = add_quote(str, &current_index, &previous_index, token);
		token = add_string(str, &current_index, &previous_index, token);
		token = plus_redire(str, &current_index, &previous_index, token);
		token = plus_token_f(str, &current_index, &previous_index, token);
		while (str[current_index] != '\0' && check_space(str[current_index]))
			current_index++;
	}
	return (token);
}

/* Counts the number of escaped characters before the given position in the
   string. Returns the count of escaped characters. */
int	calcul_escaped_charact(const char *str, int last_index)
{
	int	index;
	int	escape_count;

	index = last_index - 1;
	escape_count = 0;
	if (last_index <= 0)
		return (0);
	while (index >= 0)
	{
		if (str[index] == '\\')
		{
			escape_count++;
		}
		else
		{
			break ;
		}
		index--;
	}
	return (escape_count);
}
