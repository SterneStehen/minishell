/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:58:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:14:22 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Determines the type of the given character.
   Returns a constant indicating whether the character is a digit,
	an alphabet letter, or a printable character. */
int	find_character_type(int c)
{
	if (c >= '0' && c <= '9')
		return (IS_DIGIT);
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (IS_ALPHA);
	if ((c >= 33 && c <= 47) || (c >= 58 && c <= 59) || (c >= 63 && c <= 64)
		|| c == 61 || (c >= 91 && c <= 96) || (c >= 123 && c <= 126))
		return (IS_PRINTABLE);
	return (0);
}

/* Advances the index through the string as long as the
current character is a digit,
	alphabet letter, or printable character.
   Returns the updated index. */
int	validate_and_advance(char *str, int *index)
{
	while (str[*index] != '\0'
		&& (find_character_type(str[*index])
			& (IS_DIGIT | IS_ALPHA | IS_PRINTABLE)))
		(*index)++;
	return (*index);
}

/* Compares two strings for equality.
   Returns 1 if the strings are equal, otherwise returns 0. */
int	are_strings_equal(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (s1 == NULL || s2 == NULL)
		return (0);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (0);
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

/* Converts certain commands in the command table to lowercase
if they match specific built-in commands.*/
/* This is used to standardize commands such as "echo", "pwd", and "env". */
void	optimize_commands(t_simple_cmds *tbl)
{
	char	*temp;
	char	*cmd;

	temp = ft_strdup(tbl->command);
	cmd = tbl->command;
	convert_to_lowercase(temp, ft_strlen(temp));
	if (are_strings_equal(temp, "echo") || are_strings_equal(temp, "pwd")
		|| are_strings_equal(temp, "env"))
		convert_to_lowercase(cmd, ft_strlen(cmd));
	free(temp);
}

/* Advances the index past any whitespace characters in the string.
   Returns the updated index. */
int	advance_past_whitespace(const char *s, int idx)
{
	while (s[idx] != '\0' && check_space(s[idx]))
		idx++;
	return (idx);
}
