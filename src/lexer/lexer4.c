/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:45:58 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:34:55 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Checks if the character at the given index in the input string
is surrounded by double quotes.
   Returns 1 if the character is inside double quotes, otherwise 0. */
int	check_before(const char *input, int index)
{
	while (index-- > 1)
	{
		if (input[index] == '"')
		{
			return (1);
		}
	}
	return (0);
}

int	check_after(const char *input, int index)
{
	int	j;

	j = index;
	while (input[j] != '\0')
	{
		if (input[j] == '"')
		{
			return (1);
		}
		j++;
	}
	return (0);
}

int	inside_double_quotes(const char *input, int index)
{
	int	before;
	int	after;

	before = check_before(input, index);
	after = check_after(input, index);
	if (before && after)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

/* Checks if the input string contains any single or double quotes.
   Returns 1 if quotes are found, otherwise 0. */
int	contains_quote(const char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

/* Prints a syntax error message and returns 1. */
int	find_error_sint(void)
{
	printf("Syntax error");
	return (1);
}
