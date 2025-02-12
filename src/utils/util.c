/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 22:07:32 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 20:14:25 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Checks if the given character is a whitespace character.
   Returns 1 if the character is a whitespace character,
   otherwise returns 0. */
int	check_space(char ch)
{
	const char	*whitespace_chars = " \t\n\v\f\r";
	int			i;

	i = 0;
	while (whitespace_chars[i])
	{
		if (whitespace_chars[i] == ch)
			return (1);
		i++;
	}
	return (0);
}

void	err_exit(t_tools *tools, char *command, char *arg, int flag)
{
	if (flag == 0)
	{
		printf("%s%s: %s\n", SHELL, arg, NAR);
		tools->last_status = 255;
	}
	else
	{
		printf("%s%s: %s\n", SHELL, command, TMA);
		tools->last_status = 1;
	}
}

/* Duplicates a substring from the source string from the given
start to finish indices.
   Returns the duplicated string or NULL if memory allocation fails or
   indices are invalid. */
char	*duplicate_string_range(const char *source, int begin, int finish)
{
	int		length;
	char	*duplicate;
	int		i;

	if (begin > finish || begin < 0 || finish < 0 || source == NULL)
	{
		return (NULL);
	}
	length = finish - begin;
	duplicate = (char *)malloc(sizeof(char) * (length + 1));
	if (!duplicate)
	{
		return (NULL);
	}
	i = 0;
	while (i < length)
	{
		duplicate[i] = source[begin + i];
		i++;
	}
	duplicate[length] = '\0';
	return (duplicate);
}
