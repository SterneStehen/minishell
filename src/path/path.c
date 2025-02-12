/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 07:14:56 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 19:31:34 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	log_command(t_tools *params, char *cmd, int err_flag)
{
	if (params->flag_log == 1)
	{
		if (err_flag)
		{
			printf("%s%s: %s\n", MINI, cmd, strerror(ENOENT));
		}
		else
		{
			printf("%s%s: %s\n", MINI, cmd, "command not found");
		}
	}
	params->last_status = 127;
	return (0);
}

int	check_dot_slash(char *route, t_tools *params)
{
	if (route[0] == '.' && route[1] == '/' && strlen(route) == 2)
	{
		return (log_command(params, params->shell_string, 1));
	}
	return (-1);
}

int	check_double_dot(char *route, t_tools *params)
{
	if (route[0] == '.' && route[1] == '.' && route[2] == '/')
	{
		if (access(route, X_OK) == 0)
		{
			return (1);
		}
		else
		{
			return (log_command(params, route, 1));
		}
	}
	return (-1);
}

int	check_paths(char *route, t_tools *params)
{
	if (route[0] == '.' || route[0] == '/' || route)
	{
		if (route[0] == '.' && route[1] != '/')
		{
			return (log_command(params, route, 0));
		}
		else if (route[0] == '.' && access(route, X_OK) == 0)
		{
			return (1);
		}
		else if (route[0] == '/' && access(route, X_OK) == 0)
		{
			return (1);
		}
		else if (access(route, X_OK) == 0)
		{
			return (1);
		}
		else if (params->flag_envair == 1)
		{
			return (log_command(params, route, 1));
		}
	}
	return (0);
}

int	path_audit(char *route, t_tools *params)
{
	int	result;

	result = check_dot_slash(route, params);
	if (result != -1)
		return (result);
	result = check_double_dot(route, params);
	if (result != -1)
		return (result);
	return (check_paths(route, params));
}
