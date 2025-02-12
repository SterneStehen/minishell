/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:30:52 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/08 23:30:45 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_environment	*create_envairm(char *env_char)
{
	char			**equals;
	t_environment	*env;

	equals = split_env_string(env_char);
	if (equals == NULL)
	{
		return (NULL);
	}
	env = initialize_env_node(equals, env_char);
	free_string_array(equals);
	return (env);
}

/* Adds a new environment variable node to the end of the list. */
void	append_env_node(t_environment *head, t_environment *new)
{
	t_environment	*curr;

	curr = head;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
}

/* Initializes the environment variables from the given array.
   Splits the PATH variable into individual paths and initializes the
   environment list. */

void	setup_paths(t_tools *shell, char **env)
{
	int		i;
	char	*init_path;

	i = 0;
	init_path = NULL;
	while (env[i] != NULL)
	{
		if (strncmp(env[i], "PATH=", 5) == 0)
		{
			init_path = env[i] + 5;
			break ;
		}
		i++;
	}
	shell->paths = ft_split(init_path, ':');
}

void	setup_environment_list(t_tools *shell, char **env)
{
	int				i;
	t_environment	*head;
	t_environment	*new;

	i = 0;
	if (env[0] != NULL)
	{
		head = create_envairm(env[i++]);
		while (env[i] != NULL)
		{
			new = create_envairm(env[i++]);
			append_env_node(head, new);
		}
		shell->envair = head;
	}
	else
	{
		shell->envair = NULL;
	}
}

int	setup_environment(t_tools *shell, char **env)
{
	setup_paths(shell, env);
	setup_environment_list(shell, env);
	return (0);
}

// int	setup_environment(t_tools *shell, char **env)
// {
// 	int				i;
// 	t_environment	*head;
// 	t_environment	*new;
// 	char			*init_path;

// 	init_path = NULL;
// 	i = 0;
// 	while (env[i] != NULL)
// 	{
// 		if (strncmp(env[i], "PATH=", 5) == 0)
// 		{
// 			init_path = env[i] + 5;
// 			break ;
// 		}
// 		i++;
// 	}
// 	shell->paths = ft_split(init_path, ':');
// 	i = 0;
// 	if (env[0] != NULL)
// 	{
// 		head = create_envairm(env[i++]);
// 		while (env[i] != NULL)
// 		{
// 			new = create_envairm(env[i++]);
// 			append_env_node(head, new);
// 		}
// 		shell->envair = head;
// 	}
// 	else
// 		shell->envair = NULL;
// 	return (0);
// }
