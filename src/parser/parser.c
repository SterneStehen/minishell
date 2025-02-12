/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
		+:+     */
/*   By: smoreron <smoreron@student.42heilbronn.    +#+  +:+
		+#+        */
/*                                                +#+#+#+#+#+
		+#+           */
/*   Created: 2024/05/30 21:51:20 by smoreron          #+#    #+#             */
/*   Updated: 2024/05/30 21:51:20 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_tokens(char **tokens, int index)
{
	while (index >= 0)
	{
		free(tokens[index--]);
	}
	free(tokens);
}

int	process_and_store_token(char **tokens, char *str, int range[2],
		int *index)
{
	char	*token;
	int		start;
	int		end;

	start = range[0];
	end = range[1];
	token = process_token_segment(str, start, end);
	if (token == NULL)
	{
		free_tokens(tokens, *index);
		return (0);
	}
	tokens[++(*index)] = token;
	return (1);
}

char	**pipe_split(char *str, int index, int range[2])
{
	char	**tokens;
	int		pipes_count;

	if (str == NULL)
		return (NULL);
	pipes_count = calc_pipe_segment(str);
	tokens = (char **)malloc(sizeof(char *) * (pipes_count + 2));
	if (tokens == NULL)
		return (NULL);
	while (str[++range[1]] != '\0')
	{
		range[1] = pass_quoted_sect(str, range[1]);
		if (str[range[1]] == '|' && str[range[1] - 1] != '\''
			&& calcul_escaped_charact(str, range[1]) % 2 == 0)
		{
			if (!process_and_store_token(tokens, str, range, &index))
				return (NULL);
			range[0] = range[1] + 1;
		}
	}
	if (!process_and_store_token(tokens, str, range, &index))
		return (NULL);
	tokens[++index] = NULL;
	return (tokens);
}

/* Ignores characters inside quotes when parsing.
   Returns the updated index after the closing quote. */
int	pass_quoted_sect(char *str, int index)
{
	int	quote;

	if (str[index] != 39 && str[index] != 34)
		return (index);
	quote = str[index++];
	while (str[index] != quote && calcul_escaped_charact(str, index) % 2 == 0)
	{
		if (str[index] == '\0')
			return (index);
		index++;
	}
	return (index);
}

/* Counts the number of pipes in a string, respecting quotes.
   Returns the count of pipes. */
int	calc_pipe_segment(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = -1;
	if (str == NULL)
		return (1);
	while (str[++i] != '\0')
	{
		i = pass_quoted_sect(str, i);
		if ((size_t)i < ft_strlen(str) && str[i] != '\0' && str[i] == '|'
			&& calcul_escaped_charact(str, i) % 2 == 0)
			count++;
	}
	return (count + 1);
}
