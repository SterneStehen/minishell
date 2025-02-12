/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:37:37 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:18:39 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	extract_dollar_part(char **str, t_tools *tools, char **before_dollar,
		char **remaining)
{
	int				idx;
	t_dollar_params	params;

	params.str = str;
	params.tools = tools;
	params.before_dollar = before_dollar;
	params.remaining = remaining;
	idx = 0;
	while (str[0][idx] != '\0')
	{
		if (str[0][idx] == '$' && calcul_escaped_charact(str[0], idx) % 2 == 0
			&& bypass_expansion(str[0], idx, 0) != TRUE)
		{
			params.idx = idx;
			return (process_dollar_sign(&params));
		}
		idx++;
	}
	return (0);
}

/* Calculates the length of the string `str`.
   Returns the length of the string. */
int	calculate_length(char *str)
{
	int	length;

	length = 0;
	while (str[length] != '\0')
	{
		length++;
	}
	return (length);
}

/* Copies the content of the variable `content` into a newly allocated string.
   Returns the copied content on success, or NULL on error. */
char	*copy_data(char *content)
{
	int		len;
	char	*copied_content;
	int		index;

	len = calculate_length(content) + 1;
	copied_content = smalloc(sizeof(char) * len);
	if (!copied_content)
		return (NULL);
	index = 0;
	while (content[index] != '\0')
	{
		copied_content[index] = content[index];
		index++;
	}
	copied_content[index] = '\0';
	return (copied_content);
}

/* Allocates memory of the specified size and returns a pointer to it.
   Prints an error message and returns NULL if allocation fails. */
char	*allocate_memory(size_t size)
{
	char	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	return (ptr);
}

/* Expands variables in the input string using context's environment.
   Continues expanding until all variable references ('$') are processed. */
int	expand_variables(char **input, t_tools *context)
{
	char	**segment1;
	char	**segment2;

	while (detect_dollar_usa(*input, context))
	{
		segment1 = (char **)allocate_memory(sizeof(char *));
		segment2 = (char **)allocate_memory(sizeof(char *));
		extract_dollar_part(input, context, segment1, segment2);
		free(segment2[0]);
		free(segment1);
		free(segment2);
	}
	return (1);
}
