/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:56:43 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:17:38 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		g_global_flag = 0;

/* Ignores the specified signal.
 * This function configures the signal handler to ignore the given signal.
 * This is useful to prevent the default handling of certain signals,
 * which might be disruptive to the program's flow. */
void	ignore_signal(int signum)
{
	struct sigaction	ctrl_back_slash;

	ctrl_back_slash.sa_handler = SIG_IGN;
	ctrl_back_slash.sa_flags = SA_RESTART;
	sigemptyset(&ctrl_back_slash.sa_mask);
	sigaction(signum, &ctrl_back_slash, NULL);
}

/* Signal handler for SIGINT in the parent process.

	* This handler writes "^C\n" to the standard
	output and resets the readline buffer
	* to handle an interrupt signal (Ctrl+C)
	gracefully without exiting the program. */
void	sigint_handler_parent(int sig_num)
{
	if (sig_num == SIGINT)
	{
		write(1, "^C\n", 3);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

/* Signal handler for SIGINT in the child process.
 * This handler sets the global flag to 1 and resets the readline buffer
 * to handle an interrupt signal (Ctrl+C) in the child process. */
void	sigint_handler_child(int sig_num)
{
	if (sig_num == SIGINT)
	{
		g_global_flag = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
