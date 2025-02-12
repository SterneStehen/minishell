/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:37:37 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:28:21 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Duplicates the substring of `s` starting
from index `index` where a dollar sign ('$') is found.
   Handles different delimiters and expands the
   dollar sign segment before copying.
   Modifies `dst` to contain the duplicated substring. */

char	determine_delimiter(char **source, int idx)
{
	if (source[0][idx + 1] == '(')
		return (')');
	if (idx > 0 && source[0][idx - 1] == '\'')
		return ('\'');
	if (idx > 0 && source[0][idx - 1] == '\"')
		return ('\"');
	return (' ');
}

void	copy_segment(char **segment, char **source, int idx, char delim)
{
	int	start_pos;
	int	current_pos;

	start_pos = idx;
	current_pos = idx + 1;
	while (source[0][start_pos] != '\0' && source[0][start_pos] != delim
		&& ((source[0][current_pos] >= '0' && source[0][current_pos] <= '9')
			|| (source[0][current_pos] >= 'A'
				&& source[0][current_pos] <= 'z')))
	{
		start_pos++;
		current_pos++;
	}
	segment[0] = duplicate_string_range(source[0], idx, start_pos + 1);
}

void	copy_doll_usa(char **segment, char **source, int idx)
{
	char	delim;

	if (source[0][idx + 1] == '?')
	{
		segment[0] = duplicate_string_range(source[0], idx, idx + 2);
		return ;
	}
	delim = determine_delimiter(source, idx);
	copy_segment(segment, source, idx, delim);
}

size_t	get_length(const char *str)
{
	size_t	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}

int	combine_strings(char *dest, const char *src1, const char *src2)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (src1[i])
	{
		dest[j++] = src1[i++];
	}
	i = 0;
	while (src2[i])
	{
		dest[j++] = src2[i++];
	}
	dest[j] = '\0';
	return (0);
}
