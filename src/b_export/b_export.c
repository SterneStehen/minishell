/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 07:08:56 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 19:00:17 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_error_conditions(t_tools *tools, char **args, int idx)
{
	if (check_errors(tools, args, idx))
	{
		return (1);
	}
	if (args[idx][0] == '-')
	{
		tools->last_status = 2;
		return (1);
	}
	return (0);
}

int	process_single_argument(t_tools *tools, char **args, int idx)
{
	int	result;

	if (check_invalid_and_special_chars(args[idx]))
	{
		tools->last_status = 1;
		if (tools->flag_log == TRUE)
		{
			printf("Error: %s %s\n", args[0], args[idx]);
		}
	}
	else if (strchr(args[idx], '=') != NULL)
	{
		result = handle_equal_argument(tools, args[idx]);
		if (result != 0)
		{
			printf("Error processing argument: %s\n", args[idx]);
			tools->last_status = 1;
		}
	}
	return (0);
}

int	process_args_run(t_tools *tools, char **args)
{
	int	idx;

	idx = 1;
	while (args[idx] != NULL)
	{
		if (handle_error_conditions(tools, args, idx))
		{
			idx++;
			continue ;
		}
		process_single_argument(tools, args, idx);
		idx++;
	}
	return (0);
}

// Main export function
int	export(t_tools *tools, char *command, char **args)
{
	(void) command;
	if (args[1] == NULL)
	{
		return (print_env_vars(tools));
	}
	else if (args[1] != NULL && tools->flag_pipe == FALSE)
	{
		return (process_args_run(tools, args));
	}
	else
	{
		tools->last_status = 1;
		if (tools->flag_log == 1)
			printf("%s: wrong arguments\n", MINI);
		return (1);
	}
	return (0);
}
