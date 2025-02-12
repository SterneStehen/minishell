/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:37:37 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:33:27 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	bypass_expansion(char *input_str, int position, int backtrack_needed)
{
	int	temp_position;
	int	initial_check_result;

	initial_check_result = check_initial_conditions(input_str, position);
	if (initial_check_result != -1)
		return (initial_check_result);
	temp_position = position;
	while (temp_position != 0)
	{
		if (input_str[temp_position] == '\''
			|| input_str[temp_position] == '"')
			backtrack_needed = 0;
		temp_position--;
	}
	if (!backtrack_needed)
	{
		if (check_conditions(count_quotes(input_str, position, '\''),
				count_quotes(input_str, position, '"'), position,
				input_str))
			return (1);
	}
	return (0);
}

/* Checks if the string `str` contains a dollar sign '$'
that should trigger variable expansion.
   Updates `shell->flag_execution_completed` if a dollar
   sign is found followed by an invalid character.
   Returns TRUE if a valid dollar sign expansion is found,
	otherwise returns FALSE. */

int	is_space_after_dollar(const char *str, int i)
{
	const char	*spaces = SIMBOL;
	int			k;

	k = 0;
	while (spaces[k] != '\0')
	{
		if (spaces[k] == str[i + 1])
		{
			return (1);
		}
		k++;
	}
	return (0);
}

/* Handles conditions related to dollar signs ('$') in the input string
   based on the position and updates the context as needed.
   Returns 1 to continue the loop, -1 to stop with false, -2 to stop with true,
   or 0 if no special condition is met. */
int	process_dollar_conditions(char *input, t_tools *context, int pos)
{
	if ((input[pos] == '$' && count_escaped_chars(input, pos) % 2 != 0)
		|| (input[pos] == '$' && bypass_expansion(input, pos, 0)))
	{
		return (1);
	}
	else if (input[pos] == '$')
	{
		if (is_space_after_dollar(input, pos))
		{
			context->flag_execution_completed = 0;
			return (-1);
		}
		else
		{
			return (-2);
		}
	}
	return (0);
}

/* Checks if the input string contains a dollar sign ('$') that
   should trigger variable expansion, updating the context as needed.
   Returns 1 if a valid dollar sign is found, 0 otherwise. */
int	check_dollar_position(char *input, int pos)
{
	if (input[0] == '$' && input[pos + 1] != ' ' && input[pos + 1] != '\0')
		return (1);
	else if (input[pos] == '$' && input[pos + 1] == '\0')
		return (-1);
	return (0);
}

int	detect_dollar_usa(char *input, t_tools *context)
{
	int	pos;
	int	result;

	pos = 0;
	while (input[pos] != '\0')
	{
		result = check_dollar_position(input, pos);
		if (result == 1)
			return (1);
		else if (result == -1)
			break ;
		else
		{
			result = process_dollar_conditions(input, context, pos);
			if (result == 1)
				pos++;
			else if (result == -1)
				return (0);
			else if (result == -2)
				return (1);
		}
		pos++;
	}
	return (0);
}
