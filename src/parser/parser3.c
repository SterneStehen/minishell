/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:47:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:32:38 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Duplicates a substring from start to end in str.
   Returns the duplicated substring. */
char	*duplicate_range(char *str, int start, int end)
{
	char	*new_str;

	new_str = (char *)malloc(end - start + 1);
	if (!new_str)
	{
		printf("Memory allocation error\n");
		return (NULL);
	}
	strncpy(new_str, str + start, end - start);
	new_str[end - start] = '\0';
	return (new_str);
}

/* Processes a string token and adds it to the token list.
   Returns the updated token list. */
t_node	*add_string(char *str, int *current_index, int *previous_index,
		t_node *token)
{
	int		new_index;
	char	*new_word;

	new_index = validate_and_advance(str, current_index);
	if (new_index != *previous_index)
	{
		new_word = duplicate_range(str, *previous_index, new_index);
		token = insert_token(token, new_word, STRING);
		*previous_index = new_index;
	}
	return (token);
}

/* Finds the position of a character in a string.
   Returns the index of the character or -1 if not found. */
int	find_char_in_str(const char *src, int character)
{
	int	index;

	index = 0;
	while (src[index] != '\0')
	{
		if (src[index] == (char)character)
		{
			return (index);
		}
		index++;
	}
	if (src[index] == (char)character)
	{
		return (index);
	}
	return (-1);
}

/* Determines the type of redirection based on the substring from begin to
   finish in input. Returns the corresponding t_class value representing the
   redirection type. */
t_class	determine_redirection_type(char *input, int begin, int finish)
{
	int	length;

	length = finish - begin;
	if (length == 2)
	{
		if (input[begin] == '>')
		{
			return (GREAT_GREAT);
		}
		return (LESS_LESS);
	}
	else if (length == 1)
	{
		if (input[begin] == '>')
		{
			return (GREAT);
		}
		return (LESS);
	}
	return (UNKNOWN);
}

/* Processes a redirection token and adds it to the token list.
   Returns the updated token list. */
t_node	*plus_redire(char *input_str, int *current_index, int *previous_index,
		t_node *token_list)
{
	char	current_char;
	t_class	redirection_type;
	char	*dubl;

	if (input_str[*current_index] == '\0' || find_char_in_str(REDIR,
			input_str[*current_index]) == -1)
	{
		return (token_list);
	}
	current_char = input_str[*current_index];
	while (input_str[*current_index] == current_char
		&& input_str[*current_index] != '\0')
	{
		(*current_index)++;
	}
	redirection_type = determine_redirection_type(input_str, *previous_index,
			*current_index);
	dubl = duplicate_range(input_str, *previous_index, *current_index);
	token_list = insert_token(token_list, dubl, redirection_type);
	*previous_index = *current_index;
	return (token_list);
}
