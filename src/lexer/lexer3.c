/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:45:58 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:34:20 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Prints a syntax error message for newline and returns 1. */
int	find_error_sing_newline(void)
{
	printf("Syntax error\n");
	return (1);
}

/* Checks if the given character is a shell operator (|, <, >).
   Returns 1 if the character is an operator, otherwise 0. */
int	is_character_symbol(char c)
{
	const char	*operators = "|<>";
	int			i;

	i = 0;
	while (operators[i])
	{
		if (operators[i] == c)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

/* Checks for invalid redirection operators (e.g.,
	>>> or <<<) in the given string.
   Returns 1 and prints a syntax error message if invalid operators are found,
	otherwise returns 0. */
int	check_redir(const char *str)
{
	int	length;
	int	i;

	length = strlen(str);
	i = 0;
	if (length <= 2)
	{
		return (0);
	}
	while (i <= length - 3)
	{
		if ((str[i] == '>' && str[i + 1] == '>' && str[i + 2] == '>')
			|| (str[i] == '<' && str[i + 1] == '<' && str[i + 2] == '<'))
		{
			printf("Syntax error\n");
			return (1);
		}
		i++;
	}
	return (0);
}

/* Counts the number of escaped characters before the
given position in the string.
   Returns the count of escaped characters. */
int	count_escaped_chars(const char *str, int pos)
{
	int	count;

	count = 0;
	while (pos > 0 && str[--pos] == '\\')
	{
		count++;
	}
	return (count);
}

/* Checks if the character at the given index in the string
is preceded by a redirection operator.
   Returns 1 if the character is preceded by a redirection operator,
	otherwise 0. */
int	is_previous_char_redirection(const char *str, int index)
{
	if ((str[index - 1] == '<' || str[index - 1] == '>')
		&& str[index - 2] != '\\')
	{
		return (1);
	}
	if ((str[index - 1] == '<' || str[index - 1] == '>')
		&& str[index - 2] == '\\')
	{
		if (count_escaped_chars(str, index - 1) % 2 == 0)
		{
			return (1);
		}
	}
	return (0);
}
