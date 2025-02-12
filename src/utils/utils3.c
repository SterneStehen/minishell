/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:58:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:14:51 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Checks if the given character is an uppercase letter.
   Returns 1 if the character is uppercase, otherwise returns 0. */
int	is_capital_letter(char ch)
{
	return (ch >= 'A' && ch <= 'Z');
}

/* Converts all uppercase letters in the string to
lowercase up to the given limit.
   Returns 1 on success. */
int	convert_to_lowercase(char *s, int limit)
{
	int	len;
	int	i;

	i = 0;
	len = strlen(s);
	while (i < len && i < limit)
	{
		if (is_capital_letter(s[i]))
			s[i] = tolower(s[i]);
		i++;
	}
	return (1);
}

/* Allocates memory for a pointer to a pointer and checks
for allocation failure.
   Returns 1 on success, and 0 if memory allocation fails. */
int	safe_mall(char ***ptr, size_t size)
{
	*ptr = (char **)malloc(size);
	if (*ptr == NULL)
	{
		printf("Error: Memory allocation failed\n");
		return (0);
	}
	return (1);
}

/* Counts the number of strings in an array of strings.
   Returns the number of strings. */
int	count_strings(char **array)
{
	int	counter;

	counter = 0;
	while (array[counter] != NULL)
		counter++;
	return (counter);
}

/* Duplicates a given string.
   Returns the duplicated string or NULL if memory allocation fails. */
char	*duplicate_string(const char *str)
{
	char	*new_str;

	new_str = ft_strdup(str);
	if (new_str == NULL)
	{
		printf("Error: String duplication failed\n");
		return (NULL);
	}
	return (new_str);
}
