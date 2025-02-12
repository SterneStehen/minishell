/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:58:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:36:03 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Creates a deep copy of an array of strings.
   Returns the new array or NULL if memory allocation fails. */
char	**arr_dabl(char **array)
{
	int		index;
	int		num_strings;
	char	**new_array;

	if (array == NULL)
		return (NULL);
	num_strings = count_strings(array);
	if (!safe_mall(&new_array, sizeof(char *) * (num_strings + 1)))
		return (NULL);
	index = 0;
	while (array[index] != NULL)
	{
		new_array[index] = duplicate_string(array[index]);
		if (new_array[index] == NULL)
		{
			while (--index >= 0)
				free(new_array[index]);
			free(new_array);
			return (NULL);
		}
		index++;
	}
	new_array[index] = NULL;
	return (new_array);
}

/* Trims characters from the set at the beginning and end of the string s1.
   Returns the trimmed string or NULL if memory allocation fails. */
void	find_trim_positions(const char *s1, const char *set, size_t *begin,
		size_t *finish)
{
	size_t	j;

	*begin = 0;
	while (s1[*begin])
	{
		j = 0;
		while (set[j] && set[j] != s1[*begin])
			j++;
		if (!set[j])
			break ;
		(*begin)++;
	}
	*finish = strlen(s1);
	while (*finish > *begin)
	{
		j = 0;
		while (set[j] && set[j] != s1[*finish - 1])
			j++;
		if (!set[j])
			break ;
		(*finish)--;
	}
}

// Обрезка строки по найденным позициям и создание новой строки
char	*del_string(const char *s1, const char *set)
{
	char		*result;
	size_t		i;
	size_t		begin;
	size_t		finish;

	if (!s1 || !set)
		return (NULL);
	find_trim_positions(s1, set, &begin, &finish);
	result = (char *)malloc(sizeof(*s1) * (finish - begin + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (begin < finish)
		result[i++] = s1[begin++];
	result[i] = '\0';
	return (result);
}

/* Checks if the given string contains only space characters.
   Returns 1 if the string contains only spaces, otherwise returns 0. */
int	is_only_space(char *str)
{
	int	i;

	i = -1;
	while (str[++i] != '\0')
	{
		if (str[i] != ' ')
			return (0);
	}
	return (1);
}

/* Compares a string with another string up to the given length.
   Returns 1 if the strings are equal, otherwise returns 0. */
int	run_strncmp(t_tools *shell, char *str, int s)
{
	if (ft_strncmp(shell->shell_string, str, s))
		return (1);
	return (0);
}
