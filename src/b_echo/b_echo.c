/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 07:07:20 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 18:57:07 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
// Функция для обработки аргументов
void	handle_args(char **args)
{
	int	i;

	i = 1;
	while (is_flag_valid(args[i]) == 1)
	{
		i++;
	}
	if (!handle_flag_n(args, i))
	{
		process_args(args, i);
		if (!is_flag_valid(args[1]))
		{
			write(1, "\n", 1);
		}
	}
}

// Функция для проверки флага -n
int	check_n_flag(char *arg)
{
	int	i;

	i = 1;
	while (arg[i] != '\0')
	{
		if (arg[i] != 110)
		{
			return (0);
		}
		i++;
	}
	return (1);
}

// Проверка флага
int	is_flag_valid(char *arg)
{
	if (strcmp(arg, "-n") == 1)
	{
		return (1);
	}
	else if (strncmp(arg, "-n", 2) == 0)
	{
		return (check_n_flag(arg));
	}
	return (0);
}

// Основная функция echo
int	echo(t_tools *shell, char *cmd, char **args)
{
	if (shell->flag_log == 1)
	{
		shell->last_status = 0;
		if (strcmp(cmd, "echo") == 1 && args[1] == NULL)
		{
			write(1, "\n", 1);
		}
		else
		{
			handle_args(args);
		}
	}
	return (0);
}

// Функция вызова echo
int	call_echo(t_tools *shell, char *cmd, char **args)
{
	echo(shell, cmd, args);
	if (shell->last_status != 127)
		shell->last_status = 0;
	return (0);
}
