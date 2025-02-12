/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:37:37 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:28:27 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*savmalloc(size_t size)
{
	char	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		printf("Memory allocation failed\n");
		return (NULL);
	}
	return (ptr);
}

char	*join_strings(const char *a, const char *b)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	if (!a || !b)
	{
		printf("Invalid input\n");
		return (NULL);
	}
	len1 = get_length(a);
	len2 = get_length(b);
	result = savmalloc(len1 + len2 + 1);
	if (!result)
	{
		return (NULL);
	}
	combine_strings(result, a, b);
	return (result);
}

int	process_dollar_sign(t_dollar_params *params)
{
	char	**dollar_seg;
	char	*expanded_val;

	dollar_seg = malloc(sizeof(char *));
	if (!dollar_seg)
		return (-1);
	params->before_dollar[0] = duplicate_string_range(params->str[0], 0,
			params->idx);
	copy_doll_usa(dollar_seg, params->str, params->idx);
	expanded_val = expand_dollar_signs(dollar_seg[0], params->tools);
	params->remaining[0] = duplicate_string_range(params->str[0], params->idx
			+ strlen(*dollar_seg), strlen(params->str[0]));
	free(dollar_seg[0]);
	*dollar_seg = ft_strjoin(params->before_dollar[0], expanded_val);
	params->before_dollar[0] = join_strings(dollar_seg[0],
			params->remaining[0]);
	free(dollar_seg);
	free(params->str[0]);
	*params->str = *params->before_dollar;
	return (0);
}
