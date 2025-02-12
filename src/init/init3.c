/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:43:15 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:24:22 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Initializes the shell tools structure with default values.
   Duplicates standard input and output file descriptors and gets terminal
   settings. */
void	initialize_tools(t_tools *shell)
{
	shell->flag_envair = 0;
	shell->envp = NULL;
	shell->last_status = 0;
	shell->flag_log = FALSE;
	shell->content_hd = NULL;
	shell->commands = NULL;
	shell->shell_string = NULL;
	shell->flag_execution_completed = TRUE;
	shell->standard_input_fd = dup(0);
	shell->standard_output_fd = dup(1);
	shell->login_id = getenv("USER");
	shell->pipes = NULL;
	shell->message = NULL;
	shell->flag_pipe = 0;
	shell->flag_hd = 0;
	shell->requires_expansion = 0;
	shell->flag_ready_to_execute = 0;
	shell->flag_execution_completed = 1;
	shell->flag_log = 0;
	shell->current_pid = getpid();
	tcgetattr(0, &shell->terminal_setting);
}

/* Allocates and initializes a new t_simple_cmds structure.
   Returns the newly allocated structure or NULL if allocation fails. */
t_simple_cmds	*create_empty_structure(void)
{
	t_simple_cmds	*command_table;

	command_table = smalloc(sizeof(t_simple_cmds));
	if (command_table == NULL)
		return (NULL);
	command_table->command = NULL;
	command_table->arguments = NULL;
	command_table->token_arg = NULL;
	command_table->redirections = NULL;
	command_table->hd_file_name = NULL;
	command_table->count = -1;
	command_table->next = NULL;
	command_table->prev = NULL;
	return (command_table);
}

/* Creates a copy of the given token.
   Allocates memory for the new token and copies data from the original token.
 */
t_node	*copy_list(t_node *original_token)
{
	t_node	*new_token;

	new_token = smalloc(sizeof(t_node));
	if (new_token == NULL)
		return (NULL);
	new_token->data = strdup(original_token->data);
	new_token->class = original_token->class;
	new_token->index = original_token->index;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

/* Appends a new token to the end of the token list.
   Returns the head of the token list. */
t_node	*append_token(t_node *token_list, t_node *new_token)
{
	t_node	*current_token;

	if (token_list == NULL)
		return (new_token);
	current_token = token_list;
	while (current_token->next != NULL)
		current_token = current_token->next;
	current_token->next = new_token;
	new_token->prev = current_token;
	return (token_list);
}

/* Handles redirection tokens by cloning and appending them to the command
   table's redirections list. Returns the next token in the list. */
t_node	*process_redirection(t_simple_cmds *cmd_tbl, t_node *token,
		t_node	*new_token)
{
	new_token = copy_list(token);
	cmd_tbl->redirections = append_token(cmd_tbl->redirections, new_token);
	return (token->next);
}
