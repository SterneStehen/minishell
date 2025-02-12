/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:23:50 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 20:32:14 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	count_quotes2(char *str, int *single_quote_count,
		int *double_quote_count)
{
	int	i;

	*single_quote_count = 0;
	*double_quote_count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || (str[i] == '\\' && str[i + 1] == '\''
				&& count_escaped_chars(str, i) % 2 == 0))
		{
			if (inside_double_quotes(str, i) == 0)
			{
				(*single_quote_count)++;
			}
		}
		else if (str[i] == '\"' || (str[i] == '\\' && str[i + 1] == '\"'
				&& count_escaped_chars(str, i) % 2 == 0))
		{
			(*double_quote_count)++;
		}
		i++;
	}
}

int	check_unclosed_quotes(char *str)
{
	int	single_quote_count;
	int	double_quote_count;

	count_quotes2(str, &single_quote_count, &double_quote_count);
	if (single_quote_count % 2 != 0 || double_quote_count % 2 != 0)
	{
		printf("Syntax error\n");
		return (1);
	}
	return (0);
}

int	check_bad_pipes(char *str)
{
	int	len;

	len = strlen(str) - 1;
	if (str[0] == '|' || str[len] == '|')
	{
		printf("Syntax error\n");
		return (1);
	}
	return (0);
}

int	check_empty_prompt(char *str)
{
	return (strlen(str) == 0);
}

/* Runs the lexer to tokenize the input string and check for syntax errors.
   Returns 0 if the command is ready to execute,
	otherwise returns 1 and prints a syntax error message. */

int	run_lexer(t_tools *tools)
{
	int		index;
	int		range[2];

	index = -1;
	range[0] = 0;
	range[1] = -1;
	if (check_empty_prompt(tools->shell_string))
		return (0);
	if (check_bad_pipes(tools->shell_string))
	{
		tools->last_status = -1;
		return (1);
	}
	if (check_unclosed_quotes(tools->shell_string))
		return (1);
	if (check_wrong_operators(tools->shell_string, &(tools->last_status),
			&(tools->flag_execution_completed)))
		return (1);
	tools->flag_execution_completed = 1;
	tools->pipes = pipe_split(tools->shell_string, index, range);
	return (0);
}
