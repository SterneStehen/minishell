/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:40:53 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:33:41 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Restores the standard input file descriptor to its original state.
   Returns 0 on success, and -1 on failure with an error message. */
int	restore_standard_input(t_tools *shell)
{
	if (dup2(shell->standard_input_fd, STDIN_FILENO) == -1)
	{
		printf("Failed to restore standard input\n");
		return (-1);
	}
	return (0);
}

/* Creates a heredoc by finding the heredoc token and restoring stdin.
   Opens the heredoc file for reading and returns the file descriptor. */
int	create_heredoc_file(t_simple_cmds *table, t_tools *shell, t_node *token)
{
	int		fd;
	t_node	*heredoc_token;

	heredoc_token = locate_heredoc_token(table->redirections);
	if (heredoc_token != token)
		return (-1);
	if (restore_standard_input(shell) == -1)
		return (-1);
	fd = open(table->hd_file_name, O_RDONLY);
	if (fd == -1)
	{
		printf("Failed to open heredoc file: %s\n", table->hd_file_name);
		return (-1);
	}
	return (fd);
}

/* Opens a file for reading and returns the file descriptor. */
int	open_file_for_reading(char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	return (fd);
}

/* Opens a file for writing with the given flags and returns
the file descriptor. */
int	open_file_for_writing(char *file_name, int flags)
{
	int	fd;

	fd = open(file_name, flags, 0644);
	return (fd);
}

/* Handles errors when opening a file.
   Prints an error message and sets the last status to 1. */
int	handle_file_open_error(char *file_name, t_tools *shell)
{
	if (shell->flag_log)
		printf("%s%s: %s\n", "SHELL", file_name, strerror(errno));
	shell->last_status = 1;
	return (-1);
}
