/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:53:12 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:18:37 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Constructs the full path for a given command by combining
the base path and the command.
Allocates memory for the full path, which needs to 
be freed by the caller. */
char	*build_path(const char *base, const char *cmd)
{
	char	*slash_cmd;
	char	*full_path;

	slash_cmd = smalloc(strlen("/") + strlen(cmd) + 1);
	strcpy(slash_cmd, "/");
	strcat(slash_cmd, cmd);
	full_path = smalloc(strlen(base) + strlen(slash_cmd) + 1);
	strcpy(full_path, base);
	strcat(full_path, slash_cmd);
	free(slash_cmd);
	return (full_path);
}

/* Finds the full path of a command by searching through 
the directories in the PATH environment variable.
   Returns the full path if the command is found and executable,
	otherwise logs an error and returns NULL. */
char	*find_command_path(t_tools *tools, const char *cmd)
{
	int		idx;
	char	*path;

	idx = 0;
	if (strlen(cmd) == 0)
		return (NULL);
	if (tools->paths == NULL)
		exit(EXIT_FAILURE);
	while (tools->paths[idx] != NULL)
	{
		path = build_path(tools->paths[idx], cmd);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		idx++;
	}
	return (NULL);
}
