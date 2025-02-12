/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:27:36 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:26:17 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cmd_check_cd(t_tools *shell, char *cmd, char **args)
{
	if ((strcmp(cmd, "cd") == 0 || strcmp(cmd, C_D) == 0)
		&& shell->flag_pipe == 0)
	{
		change_directory(shell, args);
		return (1);
	}
	return (0);
}

int	cmd_check_export(t_tools *shell, char *cmd, char **args)
{
	if (strcmp(cmd, "export") == 0 && shell->flag_ready_to_execute == 1)
	{
		export(shell, cmd, args);
		return (1);
	}
	return (0);
}

int	cmd_check_pwd(t_tools *shell, char *cmd)
{
	if (strcmp(cmd, "pwd") == 0 && shell->flag_ready_to_execute == 1)
	{
		pwd(shell);
		return (1);
	}
	return (0);
}

int	cmd_check_env(t_tools *shell, char *cmd, char **args)
{
	if (strcmp(cmd, "env") == 0 && shell->flag_ready_to_execute == 1)
	{
		display_env(shell, args);
		return (1);
	}
	return (0);
}
