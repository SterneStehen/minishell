/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:54:54 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:18:13 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Checks for invalid redirections and handles them.
   Returns 1 if an invalid redirection is found and handled, otherwise 0. */
int	invalid_redirection(t_tools *shell, t_node *token, t_simple_cmds *table)
{
	t_node	*wrong;
	int		wrong_found;

	wrong = token;
	wrong_found = find_invalid_redirection(wrong);
	if (!wrong_found)
		return (0);
	perform_only_hd(shell, table, wrong);
	return (1);
}

/* Performs here documents up to the given end token.
   Processes heredoc redirections in the command table. */
void	perform_only_hd(t_tools *shll, t_simple_cmds *tabl, t_node *end)
{
	t_node	*start;
	char	*tmp;

	start = tabl->redirections;
	while (start != end && start != NULL)
	{
		if (start->class == LESS_LESS)
		{
			tmp = get_break_string(start->next->data, shll);
			free(start->next->data);
			start->next->data = tmp;
			generate_temp_heredoc(tabl, start->next->data, shll);
		}
		start = start->next;
	}
}

/* Checks if the current token has a valid redirection.
   Returns 1 if valid, otherwise 0. */
int	check_valid_redirection(t_node *curr)
{
	return (curr && (curr->class == LESS_LESS || curr->class == GREAT_GREAT
			|| curr->class == LESS || curr->class == GREAT) && curr->next
		&& curr->next->class == STRING);
}

/* Handles redirection errors.
   Sets the last status to 258 and prints an error message. */
void	handle_redirection_error(t_tools *shell)
{
	shell->last_status = 258;
	printf("Redirection error\n");
}

/* Manages redirections for a command.
   Checks and processes redirections, returns 1 if successful, otherwise 0. */
void	process_redirection_run(t_tools *shell, t_simple_cmds *table,
		t_node *current_token)
{
	int	file_descriptor;

	file_descriptor = -1;
	if (current_token->class == LESS_LESS)
	{
		file_descriptor = create_heredoc_file(table, shell, current_token);
	}
	else if (current_token->class == GREAT
		|| current_token->class == GREAT_GREAT || current_token->class == LESS)
	{
		file_descriptor = descriptor_open_file(current_token->class,
				current_token->next->data, shell);
	}
	if (file_descriptor != -1 && redirect_io(current_token->class,
			file_descriptor, shell) == FALSE)
	{
		printf("Error changing stdin/stdout\n");
		exit(EXIT_FAILURE);
	}
}
