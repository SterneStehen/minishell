/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:45:58 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:32:21 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Checks for invalid pipe characters in the given string.
   Returns 1 and prints a syntax error message if invalid pipes are found,
	otherwise returns 0. */
int	has_invalid_pipe(const char *str)
{
	int	index;

	index = 1;
	while (str[index] != '\0')
	{
		if (str[index] == '|')
		{
			if (is_previous_char_redirection(str, index))
			{
				printf("Syntax error\n");
				return (1);
			}
		}
		++index;
	}
	return (0);
}

int	check_less(char c1, char c2)
{
	if (c1 == '<' && c2 == '<')
		return (1);
	else
		return (0);
}

/* Audits the given string for invalid symbols and syntax errors.
   Returns 1 and prints a syntax error message if invalid symbols are found,
	otherwise returns 0. */
int	check_space_and_symbol(const char *str, int i)
{
	int	symbol;

	symbol = is_character_symbol(str[i - 1]);
	if (check_space(str[i]) && symbol == 1)
	{
		i = advance_past_whitespace(str, i);
		if (is_character_symbol(str[i]))
		{
			if (check_less(str[i - 2], str[i - 1]))
				return (0);
			else
			{
				printf("Syntax error\n");
				return (1);
			}
		}
	}
	return (-1);
}

int	audit_simbol_bad(const char *str)
{
	int	i;
	int	res;
	int	len;

	i = 1;
	if (has_invalid_pipe(str))
		return (1);
	while (str[i])
	{
		res = check_space_and_symbol(str, i);
		if (res != -1)
			return (res);
		i++;
	}
	len = strlen(str);
	res = is_character_symbol(str[len - 1]);
	if (res == 1)
	{
		printf("Syntax error\n");
		return (1);
	}
	return (0);
}

int	check_wrong_operators(char *str, int *last_status,
		int *flag_execution_completed)
{
	if (audit_simbol_bad(str))
	{
		*last_status = -1;
		*flag_execution_completed = 0;
		return (1);
	}
	return (0);
}
