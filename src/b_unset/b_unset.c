/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 07:10:56 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 20:26:00 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*search_char(const char *str, int ch)
{
	int	index;

	index = 0;
	while (str[index])
	{
		if (str[index] == (char)ch)
			return ((char *)(str + index));
		index++;
	}
	if (str[index] == (char)ch)
		return ((char *)(str + index));
	return (NULL);
}

int	check_invalid_chars(char *string)
{
	int	idx;

	idx = 1;
	while (string[idx])
	{
		if (string[idx] == '^')
			return (1);
		if (string[idx] == '+' || string[idx] == '=' || string[idx] == '_'
			|| (string[idx] >= 'A' && 'z' >= string[idx]) || (string[idx] >= '0'
				&& '9' >= string[idx]))
		{
			if (string[idx] == '+')
			{
				if (string[idx + 1] == '=')
					return (0);
				return (1);
			}
			else if (string[idx] == '=')
				return (0);
			idx++;
		}
		else
			return (1);
	}
	return (0);
}

int	validate_argument(char *arg)
{
	return (strlen(arg) == 0 || arg[0] == '?' || arg[0] == '$'
		|| search_char(arg, '=') || search_char(arg, '\\')
		|| check_invalid_chars(arg));
}

int	process_argument(t_tools *tools, char *arg)
{
	t_environment	*env;

	env = ft_find(tools->envair, arg);
	if (env != NULL)
	{
		tools->last_status = 0;
		remove_env_var(tools->envair, env);
		return (1);
	}
	return (0);
}
