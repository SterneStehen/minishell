/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:40:53 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:33:36 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Opens a file based on the redirection type.
   Returns the file descriptor or -1 on error. */
int	descriptor_open_file(t_class type, char *file_name, t_tools *shell)
{
	int	fd;

	fd = -1;
	if (type == LESS)
		fd = open_file_for_reading(file_name);
	else if (type == GREAT)
		fd = open_file_for_writing(file_name, O_WRONLY | O_CREAT | O_TRUNC);
	else if (type == GREAT_GREAT)
		fd = open_file_for_writing(file_name, O_WRONLY | O_CREAT | O_APPEND);
	if (fd == -1)
		return (handle_file_open_error(file_name, shell));
	return (fd);
}

/* Removes quotes from a string.
   Returns a new string without quotes. */
char	*trim_quotes(char *str)
{
	size_t	len;
	char	*new_str;
	size_t	i;
	size_t	j;

	len = strlen(str);
	new_str = smalloc(len + 1);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			new_str[j++] = str[i];
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

/* Checks for expansions in the stop word for heredoc.
   Sets the flag_hd in the shell structure. */
int	validate_expansion(char *str, t_tools *shell)
{
	size_t	i;

	i = 0;
	shell->flag_hd = 1;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			shell->flag_hd = 0;
			break ;
		}
		i++;
	}
	return (0);
}

/* Processes the stop word for heredoc.
   Returns the stop word without quotes. */
char	*get_break_string(char *str, t_tools *shell)
{
	char	*trimmed_result;

	validate_expansion(str, shell);
	trimmed_result = trim_quotes(str);
	return (trimmed_result);
}

/* Runs here documents in the command table.
   Processes and generates heredoc files. */
int	execute_heredocs(t_simple_cmds *cmd_tbl, t_tools *shell)
{
	t_node	*tk;
	char	*tmp;

	tk = cmd_tbl->redirections;
	while (tk != NULL)
	{
		if (tk->class == LESS_LESS)
		{
			validate_expansion(tk->next->data, shell);
			tmp = trim_quotes(tk->next->data);
			free(tk->next->data);
			tk->next->data = tmp;
			cmd_tbl->hd_file_name = generate_temp_heredoc(cmd_tbl,
					tk->next->data, shell);
		}
		tk = tk->next;
	}
	return (0);
}
