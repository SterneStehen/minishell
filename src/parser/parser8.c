/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:47:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:34:05 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Determines the type of redirection based on the 
	substring from start to end
   in str.
   Returns the corresponding t_class value
   representing the redirection type. */
t_class	identify_redirection_type(char *str, int start, int end)
{
	int		length;
	char	start_char;

	length = end - start;
	start_char = str[start];
	if (length == 2)
	{
		if (start_char == '>')
			return (GREAT_GREAT);
		else if (start_char == '<')
			return (LESS_LESS);
	}
	else if (length == 1)
	{
		if (start_char == '>')
			return (GREAT);
		else if (start_char == '<')
			return (LESS);
	}
	return (UNKNOWN);
}

/* Checks if a character is a special ASCII character.
   Returns 1 if the character is a special ASCII character,
		otherwise returns 0.
 */
int	is_special_ascii(char c)
{
	int	ascii;

	ascii = (int)c;
	return ((ascii >= '!' && ascii <= '/') || (ascii >= ':' && ascii <= '@')
		|| ascii == '=' || (ascii >= '[' && ascii <= '`') || (ascii >= '{'
			&& ascii <= '~'));
}

/* Allocates memory for command arguments.
   Returns the allocated array of strings or NULL if allocation fails. */
char	**allocate_cmd_args(char *cmd, t_node *token, int *size)
{
	int		initial_size;
	char	**args_array;

	initial_size = calculate_token_length(token);
	if (cmd == NULL)
	{
		*size = initial_size;
	}
	else
	{
		*size = initial_size + 1;
	}
	args_array = (char **)smalloc(sizeof(char *) * (*size + 1));
	if (args_array == NULL)
	{
		return (NULL);
	}
	args_array[*size] = NULL;
	return (args_array);
}
