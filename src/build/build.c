/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 07:11:47 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 19:07:52 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cmd_check_exit(t_tools *shell, char *cmd, char **args)
{
	if (strcmp(cmd, "exit") == 0)
	{
		exit_shell(shell, cmd, args);
		return (1);
	}
	return (0);
}

int	cmd_check_unset(t_tools *shell, char *cmd, char **args)
{
	if (strcmp(cmd, "unset") == 0 && shell->flag_ready_to_execute == 1)
	{
		unset(shell, cmd, args);
		return (1);
	}
	return (0);
}

int	cmd_check_echo(t_tools *shell, char *cmd, char **args)
{
	if (strcmp(cmd, "echo") == 0 && shell->flag_ready_to_execute == 1)
	{
		call_echo(shell, cmd, args);
		return (1);
	}
	return (0);
}

int	compare_commands(t_tools *shell, char *cmd, char **args)
{
	if (cmd_check_cd(shell, cmd, args))
		return (1);
	if (cmd_check_export(shell, cmd, args))
		return (1);
	if (cmd_check_pwd(shell, cmd))
		return (1);
	if (cmd_check_env(shell, cmd, args))
		return (1);
	if (cmd_check_exit(shell, cmd, args))
		return (1);
	if (cmd_check_unset(shell, cmd, args))
		return (1);
	if (cmd_check_echo(shell, cmd, args))
		return (1);
	return (0);
}

int	execute_builtin_commands(t_tools *shell, char *cmd, char **args)
{
	if (shell->flag_envair)
		shell->last_status = 127;
	if (compare_commands(shell, cmd, args))
		return (1);
	return (0);
}
