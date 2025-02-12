/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:20:50 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:25:34 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	update_var_data(t_tools *toolkit, t_environment *env_var, char *str,
		char *var)
{
	free(env_var->data);
	env_var->data = extract_env_data(str, var);
	toolkit->last_status = 0;
	return (0);
}

// Main function to replace variable content
int	alter_var_content(t_tools *toolkit, char *input_str, char *variable)
{
	t_environment	*current;

	current = toolkit->envair;
	while (current != NULL)
	{
		if (compare_var_title(current->title, variable))
		{
			update_var_data(toolkit, current, input_str, variable);
		}
		current = current->next;
	}
	return (0);
}

int	handle_error(const char *arg0, const char *arg)
{
	printf("%s%s: `%s': %s\n", MINI, arg0, arg, VAL);
	return (1);
}

// Function to process and print environment variables
void	print_variable(t_environment *env)
{
	if (env->data == NULL)
	{
		printf("declare -x %s\n", env->title);
	}
	else if (env->data[0] == ' ' && strlen(env->data) == 1)
	{
		printf("declare -x %s=\"\"\n", env->title);
	}
	else
	{
		printf("declare -x %s=\"%s\"\n", env->title, env->data);
	}
}

void	process_variable(t_tools *tools, t_environment *env)
{
	tools->last_status = 0;
	if (strcmp(env->title, "TERM") != 0)
	{
		print_variable(env);
	}
}
