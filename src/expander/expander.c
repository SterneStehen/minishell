/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 21:25:23 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 20:31:32 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*handle_parentheses(char *trimmed_input, t_tools *context)
{
	int		index;
	char	*empty_str;

	index = 0;
	while (trimmed_input[index] != '\0')
		index++;
	if (trimmed_input[index - 1] == ')')
	{
		return (process_parentheses(trimmed_input, context, index));
	}
	else
	{
		empty_str = allocate_memory(1);
		if (empty_str)
			empty_str[0] = '\0';
		return (empty_str);
	}
}

/* Processes a dollar sign expansion in the input string based
on the environment variables.
   Returns an allocated string with the expanded value. */
char	*run_dollar_expansion(char *trimmed_input, t_tools *context)
{
	char			*trimmed_var;
	t_environment	*env_var;
	char			*empty_str;

	trimmed_var = del_string(trimmed_input, "( )");
	env_var = ft_find(context->envair, trimmed_var);
	free(trimmed_var);
	if (!env_var)
	{
		empty_str = allocate_memory(1);
		if (empty_str)
		{
			empty_str[0] = '\0';
		}
		return (empty_str);
	}
	else
	{
		return (strdup(env_var->data));
	}
}

/* Handles the expansion of the $? expression in the input string.
   Returns a string representation of the last command's exit status. */
char	*handle_special_var(char *input, t_tools *context)
{
	int		status;
	char	*status_str;
	char	*result;

	(void) input;
	status = context->last_status;
	status_str = ft_itoa(status);
	result = status_str;
	return (result);
}

/* Prepares the final result after expanding a variable value.
   Returns an allocated string with the processed value,
	ensuring no leading space remains. */
char	*finalize_expansion(char *expanded_var)
{
	char	*var_copy;
	char	*result;

	var_copy = copy_variable(expanded_var);
	result = var_copy;
	if (result[0] == ' ' && result[1] == '\0')
	{
		free(result);
		result = allocate_memory(1);
		if (result)
		{
			result[0] = '\0';
		}
	}
	return (result);
}

/* Expands dollar sign expressions in the input stringbased
on context's environment.
   Returns an allocated string with the fully expanded result. */
char	*expand_dollar_signs(char *input, t_tools *context)
{
	char	*expanded_var;
	char	*result;
	char	*trimmed_input;

	expanded_var = NULL;
	result = NULL;
	if (input[1] == '?')
		return (handle_special_var(input, context));
	else
	{
		trimmed_input = del_string(input, "$");
		if (trimmed_input[0] == '(')
			expanded_var = handle_parentheses(trimmed_input, context);
		else
			expanded_var = run_dollar_expansion(trimmed_input, context);
		free(trimmed_input);
		if (!expanded_var || *expanded_var == '\0')
			return (expanded_var);
		result = finalize_expansion(expanded_var);
	}
	return (result);
}
