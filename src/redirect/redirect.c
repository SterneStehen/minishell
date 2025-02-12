/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 08:00:44 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 19:32:46 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	manage_redirections(t_tools *shell, t_simple_cmds *table)
{
	t_node	*token;
	int		valid;

	token = table->redirections;
	while (token != NULL)
	{
		valid = check_valid_redirection(token);
		if (!valid)
		{
			handle_redirection_error(shell);
			return (0);
		}
		process_redirection_run(shell, table, token);
		if (token->next)
		{
			token = token->next->next;
		}
		else
		{
			token = NULL;
		}
	}
	return (1);
}

/* Duplicates a file descriptor and closes the old one.
   Returns the result of dup2. */
int	duplicate_and_close(int oldfd, int newfd)
{
	int	result;

	result = dup2(oldfd, newfd);
	close(oldfd);
	return (result);
}

/* Processes a here document.
   Duplicates the file descriptor to STDIN and unlinks the temporary file. */
int	process_heredoc_input(int fd)
{
	int	ret_val;

	ret_val = duplicate_and_close(fd, STDIN_FILENO);
	unlink("/tmp/heredoc.XXXXXX");
	return (ret_val);
}

/* Redirects stdin and stdout based on the redirection type.
//    Handles different redirection types and processes heredocs. */

int	process_input(int fd, t_tools *shell)
{
	if (fd == -1)
	{
		shell->last_status = errno;
		printf("Error: %s\n", strerror(errno));
		return (0);
	}
	return (duplicate_and_close(fd, STDIN_FILENO));
}

/* Redirects stdin and stdout based on the redirection type.
//    Handles different redirection types and processes heredocs. */

int	redirect_io(t_class type, int fd, t_tools *shell)
{
	int	result;

	result = 0;
	if (type == LESS_LESS)
	{
		result = process_heredoc_input(fd);
	}
	else if (type == LESS)
	{
		result = process_input(fd, shell);
	}
	else if (type == GREAT || type == GREAT_GREAT)
	{
		result = duplicate_and_close(fd, STDOUT_FILENO);
	}
	if (result == -1)
	{
		shell->last_status = errno;
		printf("Error: %s\n", strerror(errno));
		return (0);
	}
	return (1);
}
