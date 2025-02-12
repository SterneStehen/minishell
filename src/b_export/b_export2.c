/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:20:50 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:25:22 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	iterate_variables(t_tools *tools)
{
	t_environment	*env_node;

	env_node = tools->envair;
	while (env_node != NULL)
	{
		process_variable(tools, env_node);
		env_node = env_node->next;
	}
}

int	print_env_vars(t_tools *tools)
{
	if (tools == NULL || tools->envair == NULL)
	{
		fprintf(stderr, "No environment variables to display.\n");
		return (-1);
	}
	iterate_variables(tools);
	return (0);
}

// Function to validate and process arguments
int	check_errors(t_tools *tools, char **args, int idx)
{
	if (args[0][0] == '=' || ft_strlen(args[idx]) == 0)
	{
		tools->last_status = 1;
		if (tools->flag_log == TRUE)
		{
			printf("Error: %s %s\n", args[0], args[idx]);
		}
		return (1);
	}
	return (0);
}

// Проверка на недопустимые символы и знаки
int	check_invalid_and_special_chars(char *arg)
{
	int	plus;
	int	idx;

	plus = FALSE;
	idx = 0;
	while (arg[idx] != '\0')
	{
		if (arg[idx] == '+')
		{
			if (arg[idx + 1] == '=')
			{
				plus = FALSE;
				break ;
			}
			plus = TRUE;
			break ;
		}
		idx++;
	}
	if (isdigit(arg[0]) || strchr(arg, '\\')
		|| check_invalid_chars(arg) == TRUE || plus)
	{
		return (1);
	}
	return (0);
}

int	handle_equal_argument(t_tools *tools, char *arg)
{
	char	**split_var;

	split_var = ft_split(arg, '=');
	if (!split_var)
	{
		return (1);
	}
	if (ft_find(tools->envair, split_var[0]) == NULL)
	{
		insert_new_variable(tools, arg);
	}
	else
	{
		alter_var_content(tools, arg, split_var[0]);
	}
	free_string_array(split_var);
	return (0);
}
