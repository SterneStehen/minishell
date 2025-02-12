/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 22:50:00 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 19:33:55 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Sets up a signal handler for SIGINT using the specified handler function.

	* This function configures the signal handler
	for SIGINT to use the given handler function,
	* allowing for custom behavior when the
	interrupt signal (Ctrl+C) is received. */
void	setup_sigint_handler(void (*handler)(int))
{
	struct sigaction	ctrl_c;

	ctrl_c.sa_handler = handler;
	ctrl_c.sa_flags = SA_RESTART;
	sigemptyset(&ctrl_c.sa_mask);
	sigaction(SIGINT, &ctrl_c, NULL);
}

/* Configures the terminal settings to disable the ECHOCTL flag.

	* This function modifies the terminal
	settings to prevent the echoing of control characters,
 * such as Ctrl+C, to make the terminal interaction cleaner. */
void	configure_termios(struct termios *term_settings)
{
	struct termios	new_settings;

	tcgetattr(1, term_settings);
	tcgetattr(1, &new_settings);
	new_settings.c_lflag &= ~ECHOCTL;
	tcsetattr(1, TCSAFLUSH, &new_settings);
}

/* Configures signals for the child process.
 * Restores the terminal settings, ignores SIGQUIT,
	and sets the SIGINT handler to default.

	* This ensures that the child process behaves correctly
	with respect to terminal signals. */
void	config_signals_child(struct termios *term_settings)
{
	tcsetattr(1, TCSAFLUSH, term_settings);
	ignore_signal(SIGQUIT);
	setup_sigint_handler(SIG_DFL);
}

/* Configures signals for the parent process.
 * Sets up the SIGINT handler and ignores SIGQUIT.

	* This ensures that the parent process handles
	signals appropriately during its execution. */
void	configure_signals_parent(void)
{
	setup_sigint_handler(sigint_handler_parent);
	ignore_signal(SIGQUIT);
}

/* Configures signals for the child process including terminal settings.
 * Sets up the SIGINT handler, ignores SIGQUIT, and resets the global flag.
 * This function is used to prepare the child process for signal handling. */
void	configure_signals(struct termios *term_settings)
{
	g_global_flag = 0;
	configure_termios(term_settings);
	ignore_signal(SIGQUIT);
	setup_sigint_handler(sigint_handler_child);
}
