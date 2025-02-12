/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:37:37 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:31:37 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Processes a command string by expanding
variables and converting to lowercase.
   Returns 1 if the command matches "awk", otherwise returns 0. */
int	run_process_command(char *cmd, t_tools *context)
{
	char	*temp_str;
	int		j;

	if (cmd != NULL)
	{
		if (cmd[0] == '$')
			expand_variables(&cmd, context);
		temp_str = ft_strdup(cmd);
		j = 0;
		while (temp_str[j] != '\0')
		{
			temp_str[j] = tolower(temp_str[j]);
			j++;
		}
		if (are_strings_equal(temp_str, "awk") == 0)
		{
			return (free(temp_str), 1);
		}
		else
			return (free(temp_str), 0);
	}
	return (0);
}

/* Processes tokens by expanding variables in each token's data field. */
void	process_tokens(t_node *tokens, t_tools *context)
{
	while (tokens != NULL)
	{
		expand_variables(&tokens->data, context);
		tokens = tokens->next;
	}
}

/* Expands variables in multiple command tables and their respective tokens. */
int	multiple_tables_expand(t_simple_cmds *commands, t_tools *context)
{
	char	*temp_result;

	while (commands != NULL)
	{
		temp_result = NULL;
		if (run_process_command(commands->command, context))
		{
			process_tokens(commands->token_arg, context);
			process_tokens(commands->redirections, context);
		}
		commands = commands->next;
		free(temp_result);
	}
	return (1);
}

/* Copies a variable value to a newly allocated string.
   Returns an empty string if the input is NULL or empty. */
char	*copy_variable(const char *var_val)
{
	char	*empty_str;
	char	*copied_val;

	if (!var_val || *var_val == '\0')
	{
		empty_str = allocate_memory(sizeof(char *));
		if (empty_str)
		{
			empty_str[0] = '\0';
		}
		return (empty_str);
	}
	copied_val = strdup(var_val);
	return (copied_val);
}

/* Processes a string enclosed in parentheses to find and
return its value from the environment.
   Returns an empty string if the variable is not found in the environment. */

char	*process_parentheses(char *trimmed_input, t_tools *context, int index)
{
	char			*trimmed_var;
	t_environment	*env_var;
	char			*empty_str;

	(void) index;
	trimmed_var = del_string(trimmed_input, "( )");
	env_var = ft_find(context->envair, trimmed_var);
	free(trimmed_var);
	if (!env_var)
	{
		empty_str = allocate_memory(sizeof(char *));
		if (empty_str)
			empty_str[0] = '\0';
		return (empty_str);
	}
	else
	{
		return (strdup(env_var->data));
	}
}
