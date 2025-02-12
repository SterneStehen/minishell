/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:47:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:18:44 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Checks if a given token is a redirection token.
   Returns 1 if it is a redirection token, otherwise returns 0. */
int	is_redirection_token(t_node *tkn)
{
	if (!tkn)
		return (0);
	if (tkn->class == LESS_LESS || tkn->class == GREAT_GREAT
		|| tkn->class == LESS || tkn->class == GREAT)
		return (1);
	return (0);
}

/* Frees the memory allocated for a single token. */
void	free_single_token(t_node *tkn)
{
	if (!tkn)
		return ;
	free(tkn->data);
	free(tkn);
}

/* Compares two strings for equality.
   Returns 1 if the strings are equal, otherwise returns 0. */
int	strings_equal(char *str1, char *str2)
{
	int		index;

	index = 0;
	if (!str1 || !str2)
		return (0);
	if (strlen(str1) != strlen(str2))
		return (0);
	while (str1[index] != '\0' || str2[index] != '\0')
	{
		if (str1[index] != str2[index])
			return (0);
		index++;
	}
	return (1);
}

/* Removes quotes from the input string if present.
   Returns the new string without quotes. */
char	*remove_quotes(char *input)
{
	char	*result;

	if (input[0] == '\'' || input[0] == '\"')
	{
		result = duplicate_string_range(input, 1, strlen(input) - 1);
		return (result);
	}
	return (strdup(input));
}

/* Processes tokens by removing quotes from their data.
   Skips processing for HERE_DOC tokens. */
void	manage_process_tokens(t_node *tokens)
{
	char	*temp;

	while (tokens != NULL)
	{
		if (tokens->class == LESS_LESS)
		{
			tokens = tokens->next;
			if (tokens != NULL)
				tokens = tokens->next;
			if (tokens == NULL)
				return ;
		}
		temp = remove_quotes(tokens->data);
		free(tokens->data);
		tokens->data = temp;
		tokens = tokens->next;
	}
}
