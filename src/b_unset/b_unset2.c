/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_unset2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:26:03 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:27:30 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_argument(t_tools *tools, char **args, int index)
{
	if (validate_argument(args[index]))
	{
		tools->last_status = 1;
		if (tools->flag_log)
		{
			printf("%s%s: `%s': %s\n", MINI, args[0], args[index], VAL);
		}
		return (1);
	}
	else if (args[index][0] == '-')
	{
		tools->last_status = 2;
		return (0);
	}
	process_argument(tools, args[index]);
	return (0);
}

int	unset(t_tools *tools, char *command, char **args)
{
	int	index;

	(void)command;
	index = 1;
	if (args[1] == NULL)
	{
		return (0);
	}
	while (args[index] != NULL)
	{
		if (handle_argument(tools, args, index))
		{
			return (1);
		}
		if (args[index][0] == '-')
		{
			index++;
			continue ;
		}
		index++;
	}
	return (0);
}

t_environment	*ft_find(t_environment *env, const char *name)
{
	while (env != NULL)
	{
		if (strncmp(env->title, name, strlen(name)) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	remove_env_var(t_environment *start, t_environment *target)
{
	t_environment	*temporary_node;
	t_environment	*current_node;

	current_node = start;
	while (current_node != NULL)
	{
		if (current_node->next == target)
		{
			temporary_node = target->next;
			current_node->next = temporary_node;
			free(target->title);
			free(target->data);
			free(target);
			return (0);
		}
		current_node = current_node->next;
	}
	return (0);
}
