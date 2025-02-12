/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:29:14 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:11:06 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/* Free memory allocated for an array of strings. */
void	free_string_array(char **str)
{
	int	i;

	if (str == NULL)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

/* Free memory allocated for the environment structure. */
void	release_env_list(t_environment *env_list)
{
	t_environment	*temp;

	while (env_list)
	{
		temp = env_list->next;
		free(env_list->title);
		free(env_list->data);
		free(env_list);
		env_list = temp;
	}
}

/* Frees the memory allocated for all tokens in a linked list. */
void	clear_token_list(t_node *tkn)
{
	t_node	*temp;

	temp = NULL;
	while (tkn)
	{
		temp = tkn->next;
		free_single_token(tkn);
		tkn = temp;
	}
}

/* Free memory allocated for the command tables. */
void	cleanup_command_tables(t_simple_cmds *cmd_tables)
{
	t_simple_cmds	*next_table;
	t_node			*temp;

	while (cmd_tables)
	{
		while (cmd_tables->token_arg)
		{
			temp = cmd_tables->token_arg->next;
			free_single_token(cmd_tables->token_arg);
			cmd_tables->token_arg = temp;
		}
		while (cmd_tables->redirections)
		{
			temp = cmd_tables->redirections->next;
			free_single_token(cmd_tables->redirections);
			cmd_tables->redirections = temp;
		}
		free_string_array(cmd_tables->arguments);
		free(cmd_tables->command);
		free(cmd_tables->hd_file_name);
		next_table = cmd_tables->next;
		free(cmd_tables);
		cmd_tables = next_table;
	}
}
