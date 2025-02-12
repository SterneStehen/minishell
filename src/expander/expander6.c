/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:37:37 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:19:54 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Checks if a count is odd.
   Returns 1 if odd, 0 otherwise. */
int	is_odd(int count)
{
	return (count % 2);
}

/* Counts the number of occurrences of the specified quote character
   in the string `str` up to position `pos`.
   Returns the count of quote characters. */

int	count_quotes(char *str, int pos, char quote_char)
{
	int	count;

	count = 0;
	while (pos >= 0)
	{
		if (str[pos] == quote_char)
		{
			count++;
		}
		pos--;
	}
	return (count);
}

/* Checks conditions based on the counts of single and double quotes,
   the initial position, and the input string.
   Returns 1 if conditions are met for processing, 0 otherwise. */
int	check_conditions(int sq_count, int dq_count, int initial_position,
		char *input_str)
{
	if (is_odd(sq_count) && !is_odd(dq_count))
		return (1);
	else if (is_odd(sq_count) && is_odd(dq_count)
		&& input_str[initial_position - 1] == '"')
		return (1);
	else if (!is_odd(sq_count) && is_odd(dq_count))
		return (0);
	else if (is_odd(sq_count) && dq_count % 4 == 2
		&& input_str[initial_position - 1] == '\'')
		return (1);
	else if (!is_odd(sq_count) && dq_count % 4 == 2
		&& input_str[initial_position - 1] == '\''
		&& input_str[initial_position - 3] == '\'')
		return (1);
	else if (sq_count % 4 == 0 && dq_count % 4 == 3)
		return (1);
	return (0);
}

/* Determines if expansion should be skipped based on the input string
   and position.
   Returns 1 if expansion should be skipped, 0 otherwise. */
int	check_initial_conditions(char *input_str, int position)
{
	int	sq_count;
	int	dq_count;
	int	initial_position;

	initial_position = position;
	if (position == 0)
	{
		return (0);
	}
	if (position > 0 && (input_str[position + 1] == '"'
			|| input_str[position + 1] == '\''))
	{
		return (1);
	}
	sq_count = count_quotes(input_str, position, '\'');
	dq_count = count_quotes(input_str, position, '"');
	if (check_conditions(sq_count, dq_count, initial_position, input_str))
	{
		return (1);
	}
	return (-1);
}
