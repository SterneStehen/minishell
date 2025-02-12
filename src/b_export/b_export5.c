/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:20:50 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:25:50 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	calc_lengths(char *str1, char *str2)
{
	return (strlen(str1) - strlen(str2));
}

void	copy_content(char *src, char *dest, int start_idx)
{
	int	idx;

	idx = 0;
	while (src[start_idx + idx] != '\0')
	{
		dest[idx] = src[start_idx + idx];
		idx++;
	}
	dest[idx] = '\0';
}
