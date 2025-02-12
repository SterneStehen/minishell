/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:13:59 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:24:46 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_words(char *input_str, char delimiter)
{
	int	in_word;
	int	word_count;
	int	index;

	in_word = 0;
	word_count = 0;
	index = 0;
	while (input_str[index] != '\0')
	{
		if (input_str[index] == delimiter)
		{
			in_word = 0;
		}
		else if (input_str[index] != delimiter && in_word == 0)
		{
			word_count++;
			in_word = -1;
		}
		index++;
	}
	return (word_count);
}

int	initialize_oldpwd(t_tools *shell)
{
	if (ft_find(shell->envair, "OLDPWD") == 0)
	{
		insert_new_variable(shell, "OLDPWD");
	}
	return (0);
}

int	update_environment_vars(t_tools *shell, char *old_pwd)
{
	char	*new_pwd;
	char	*pwd_combined;
	char	*old_pwd_combined;

	new_pwd = getcwd(NULL, PATH_MAX);
	pwd_combined = smalloc(strlen("PWD=") + ft_strlen(new_pwd) + 1);
	old_pwd_combined = smalloc(strlen("OLDPWD=") + ft_strlen(old_pwd) + 1);
	printf(pwd_combined, "PWD=%s", new_pwd);
	printf(old_pwd_combined, "OLDPWD=%s", old_pwd);
	alter_var_content(shell, old_pwd_combined, "OLDPWD");
	alter_var_content(shell, pwd_combined, "PWD");
	shell->last_status = shell->last_status;
	free(old_pwd_combined);
	free(pwd_combined);
	free(new_pwd);
	return (0);
}

int	execute_cd_home(t_tools *shell)
{
	t_environment	*home_dir;

	home_dir = ft_find(shell->envair, "HOME");
	if (!home_dir)
	{
		shell->last_status = 1;
		if (shell->flag_log)
		{
			printf("%scd: homeless\n", SHELL);
		}
	}
	else if (strlen(home_dir->data) < 1)
	{
		printf("\n");
	}
	else if (chdir(home_dir->data) == -1 && shell->flag_log)
	{
		printf("%scd: %s: %s\n", SHELL, home_dir->data, strerror(ENOENT));
	}
	return (0);
}

int	execute_cd_tilde(t_tools *shell, char *argument)
{
	t_environment	*home;
	char			*path;
	char			*tilde_trimmed;

	if (shell->flag_envair == 0)
	{
		home = ft_find(shell->envair, "HOME");
		tilde_trimmed = del_string(argument, "~");
		path = smalloc(strlen(home->data) + strlen(tilde_trimmed) + 1);
		sprintf(path, "%s%s", home->data, tilde_trimmed);
	}
	else
	{
		path = smalloc(strlen("/Users/") + strlen(shell->login_id) + 1);
		sprintf(path, "/Users/%s", shell->login_id);
	}
	if (chdir(path) == -1)
	{
		if (shell->flag_log)
			return (printf("%scd: %s: %s\n", SHELL, path, strerror(errno)));
	}
	free(tilde_trimmed);
	free(path);
	return (0);
}
