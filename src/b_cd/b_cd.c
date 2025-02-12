/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 07:06:03 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 18:53:49 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_cd_oldpwd(t_tools *shell)
{
	t_environment	*old_pwd;

	old_pwd = ft_find(shell->envair, "OLDPWD");
	if (!old_pwd)
	{
		shell->last_status = 1;
		return (printf("%scd: %s: %s\n", SHELL, "OLDPWD", strerror(errno)));
	}
	else if (old_pwd->data == NULL)
	{
		printf("\n");
	}
	else if (chdir(old_pwd->data) == -1 && shell->flag_log)
	{
		return (printf("%scd: %s: %s\n", SHELL, old_pwd->data,
				strerror(errno)));
	}
	else if (shell->flag_log)
	{
		printf("%s\n", old_pwd->data);
	}
	return (0);
}

int	execute_cd_back(t_tools *shell, char **args)
{
	if (strcmp(args[1], "..") == 0 && args[2] == NULL)
	{
		chdir(args[1]);
	}
	else if (chdir(args[1]) == -1 && shell->flag_log)
	{
		return (printf("%scd: %s: %s\n", SHELL, args[2], strerror(errno)));
	}
	return (0);
}

int	update_directory_vars(t_tools *shell, char *old_pwd_content)
{
	t_environment	*pwd;
	t_environment	*oldpwd;

	pwd = ft_find(shell->envair, "PWD");
	if (pwd)
	{
		pwd->data = getcwd(NULL, 0);
	}
	oldpwd = ft_find(shell->envair, "OLDPWD");
	if (oldpwd)
	{
		oldpwd->data = old_pwd_content;
	}
	return (0);
}

int	change_directory(t_tools *shell, char **args)
{
	char			*old_pwd_content;
	t_environment	*old_pwd;

	old_pwd = ft_find(shell->envair, "PWD");
	old_pwd_content = old_pwd->data;
	if (args[1] == NULL)
		execute_cd_home(shell);
	else if (args[1][0] == '~')
		execute_cd_tilde(shell, args[1]);
	else if (are_strings_equal(args[1], "-"))
		execute_cd_oldpwd(shell);
	else if (args[1] != NULL && strcmp(args[1], "..") != 0 && args[1][0] != '-')
	{
		if (chdir(args[1]) == -1)
		{
			shell->last_status = 1;
			if (shell->flag_log)
				return (printf("%scd: %s: %s\n", SHELL, args[1],
						strerror(errno)));
		}
	}
	else if (strcmp(args[1], "..") == 0 || strncmp(args[1], "../", 3) == 0)
		execute_cd_back(shell, args);
	update_directory_vars(shell, old_pwd_content);
	return (0);
}
