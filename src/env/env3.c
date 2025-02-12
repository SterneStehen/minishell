/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:30:52 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:56:45 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Initializes a new environment variable node from the given string.
   Splits the string by '=' to separate the variable name and value. */

char	**split_env_string(char *env_char)
{
	return (ft_split(env_char, '='));
}

/* Updates the environment variables in the shell structure.
   Frees the existing environment array and converts the linked list to an array
   of strings. */
void	refresh_environment(t_tools *shell)
{
	if (shell->envp != NULL)
		free_string_array(shell->envp);
	shell->envp = convert_env_list_to_array(shell->envair);
}
