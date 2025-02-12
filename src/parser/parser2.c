/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:47:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:32:32 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Processes a flag token and adds it to the token list.
   Returns the updated token list. */
t_node	*process_flag_token(char *str, int *idx, int *prev_idx,
		t_node *token_list)
{
	char	*substring;
	int		start_idx;

	if (str[*idx] != FLAG)
	{
		return (token_list);
	}
	(*idx)++;
	start_idx = *idx;
	while (str[*idx] != '\0' && str[*idx] != SPACE)
	{
		if ((find_character_type(str[*idx])
				& (IS_ALPHA | IS_DIGIT | IS_PRINTABLE)) == 0)
		{
			break ;
		}
		(*idx)++;
	}
	if (*idx > start_idx)
	{
		substring = duplicate_string_range(str, *prev_idx, *idx);
		token_list = insert_token(token_list, substring, 1);
	}
	*prev_idx = *idx;
	return (token_list);
}

/* Processes a flag token and adds it to the token list.
   Returns the updated token list. */
t_node	*plus_token_f(char *input_str, int *current_index, int *previous_index,
		t_node *token_list)
{
	int	idx;
	int	prev_idx;

	idx = *current_index;
	prev_idx = *previous_index;
	token_list = process_flag_token(input_str, &idx, &prev_idx, token_list);
	*current_index = idx;
	*previous_index = prev_idx;
	return (token_list);
}

/* Parses the input string into tokens and command tables.
   Returns 1 if an error occurs, otherwise returns 0. */
int	parse_shell_input(t_tools *shell)
{
	int				i;
	t_node			*token;
	t_simple_cmds	*cmd_tbls;

	i = 0;
	cmd_tbls = NULL;
	while (shell->pipes[i] != NULL)
	{
		token = NULL;
		token = tokenize_slip_string(shell->pipes[i++], token);
		cmd_tbls = create_command_table(cmd_tbls, token);
		clear_token_list(token);
	}
	multiple_tables_expand(cmd_tbls, shell);
	delite_quotes_tables(cmd_tbls, shell);
	init_commands(cmd_tbls);
	free_string_array(shell->pipes);
	shell->commands = cmd_tbls;
	if (audit_command(shell->commands))
		return (1);
	else
		return (0);
}

/* Audits the command tables to ensure they are valid.
   Returns TRUE if an error is found, otherwise returns FALSE. */
int	audit_command(t_simple_cmds *commands)
{
	t_node	*token;
	int		result;

	result = 0;
	while (commands != NULL)
	{
		token = commands->redirections;
		if (token == NULL)
			result = 1;
		while (token != NULL)
		{
			if (token->class == STRING)
				return (1);
			token = token->next;
		}
		commands = commands->next;
	}
	return (result);
}

/* Processes a substring from start to end, removing leading and trailing
   spaces. Returns the processed substring. */
char	*process_token_segment(char *str, int start, int end)
{
	char	*tmp;
	char	*trimmed;

	tmp = duplicate_string_range(str, start, end);
	trimmed = del_string(tmp, SIMBOL);
	free(tmp);
	return (trimmed);
}
