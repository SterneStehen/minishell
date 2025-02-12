/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 21:17:37 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 14:25:32 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Retrieves the current directory from the PWD environment variable.
   If the PWD is root, returns "/",
		otherwise returns the last directory in the path. */
char	*get_current_directory(t_tools *shell)
{
	int				i;
	t_environment	*pwd;
	char			**split;
	char			*directory;

	i = 0;
	pwd = ft_find(shell->envair, "PWD");
	if (strcmp(pwd->data, "/") == TRUE)
		directory = ft_strdup("/");
	else
	{
		split = ft_split(pwd->data, '/');
		while (split[i] != NULL)
			i++;
		directory = ft_strdup(split[--i]);
		free_string_array(split);
	}
	return (directory);
}

t_environment	*initialize_env_node(char **equals, char *env_char)
{
	t_environment	*env;

	env = malloc(sizeof(t_environment));
	if (env == NULL)
	{
		return (NULL);
	}
	env->title = ft_strdup(equals[0]);
	if (env->title == NULL)
	{
		free(env);
		return (NULL);
	}
	env->data = extract_env_data(env_char, equals[0]);
	if (env->data == NULL)
	{
		free(env->title);
		free(env);
		return (NULL);
	}
	env->next = NULL;
	return (env);
}

/* Prints the environment variables if the shell's flag_log is set to TRUE.
   Handles cases where the environment variable's value is a single space
   character. */
void	display_env(t_tools *shell, char **args)
{
	t_environment	*curr;

	if (args[1] != NULL)
		return ;
	curr = shell->envair;
	if (shell->flag_log == TRUE)
	{
		while (curr != NULL)
		{
			shell->last_status = 0;
			if (curr->data != NULL)
			{
				if (curr->data[0] == ' ' && ft_strlen(curr->data) == 1)
				{
					printf("%s=\n", curr->title);
				}
				else
				{
					printf("%s=%s\n", curr->title, curr->data);
				}
			}
			curr = curr->next;
		}
	}
}

char	*create_env_string(t_environment *node)
{
	char	*tmp;
	char	*full_env_str;

	tmp = ft_strjoin(node->title, "=");
	if (node->data != NULL)
	{
		full_env_str = ft_strjoin(tmp, node->data);
		free(tmp);
	}
	else
	{
		full_env_str = tmp;
	}
	return (full_env_str);
}

/* Converts the linked list of environment variables to an array of strings.
//    Each string is formatted as "name=value". */

char	**convert_env_list_to_array(t_environment *env)
{
	char			**env_array;
	int				list_size;
	int				index;
	t_environment	*current_node;

	list_size = 0;
	current_node = env;
	while (current_node != NULL)
	{
		current_node = current_node->next;
		list_size++;
	}
	env_array = malloc(sizeof(char *) * (list_size + 1));
	if (env_array == NULL)
		return (NULL);
	env_array[list_size] = NULL;
	current_node = env;
	index = 0;
	while (current_node != NULL && index < list_size)
	{
		env_array[index] = create_env_string(current_node);
		current_node = current_node->next;
		index++;
	}
	return (env_array);
}

// char	**convert_env_list_to_array(t_environment *env) {
//   char *tmp;
//   char *full_env_str;
//   char **env_array;
//   int list_size;
//   int index;
//   t_environment *current_node;

//   list_size = 0;
//   current_node = env;
//   while (current_node != NULL) {
//     current_node = current_node->next;
//     list_size++;
//   }
//   env_array = malloc(sizeof(char *) * (list_size + 1));
//   if (env_array == NULL)
//     return (NULL);
//   env_array[list_size] = NULL;
//   current_node = env;
//   index = 0;
//   while (current_node != NULL && index < list_size) {
//     tmp = ft_strjoin(current_node->title, "=");
//     if (current_node->data != NULL) {
//       full_env_str = ft_strjoin(tmp, current_node->data);
//       free(tmp);
//     } else {
//       full_env_str = tmp;
//     }
//     env_array[index] = full_env_str;
//     current_node = current_node->next;
//     index++;
//   }
//   return (env_array);
// }
