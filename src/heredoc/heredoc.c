/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 07:13:43 by smoreron          #+#    #+#             */
/*   Updated: 2024/07/09 19:21:03 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Cleans up the specified filename by unlinking it.
   Returns 0 on success. */
int	clean_filename(char *filename)
{
	unlink(filename);
	return (0);
}

/* Concatenates two strings and returns the result.
   Allocates memory for the new string. */
char	*concatenate_strings(char *str1, char *str2)
{
	char	*result;

	result = (char *)smalloc(strlen(str1) + strlen(str2) + 1);
	strcpy(result, str1);
	strcat(result, str2);
	return (result);
}

/* Generates a unique filename for heredoc.
   Cleans up the previous filename if it exists. */
char	*generate_temp_filename(t_simple_cmds *table)
{
	char	*base_path;
	char	*extension;
	char	*part1;
	char	*part2;
	char	*index_str;

	base_path = "/tmp/heredoc";
	extension = ".XXXXXX";
	if (table->hd_file_name != NULL)
	{
		clean_filename(table->hd_file_name);
		return (table->hd_file_name);
	}
	index_str = ft_itoa(table->count);
	part1 = concatenate_strings(base_path, index_str);
	free(index_str);
	part2 = concatenate_strings(part1, extension);
	free(part1);
	clean_filename(part2);
	return (part2);
}

/* Writes user input to the heredoc file until the stop word is encountered.
   Expands variables in the input if flag_hd is set. */
int	write_to_file(int fd, char *stop_word, t_tools *shell)
{
	char	*input;
	size_t	len;

	while (1)
	{
		write(0, "> ", 2);
		input = get_next_line(STDIN_FILENO);
		len = strlen(stop_word);
		if (input == NULL || (ft_strncmp(input, stop_word, len) == 0
				&& ((strlen(input) - 1) == len)))
		{
			free(input);
			break ;
		}
		if (shell->flag_hd == 1)
			expand_variables(&input, shell);
		write(fd, input, ft_strlen(input));
		free(input);
	}
	return (0);
}

/* Generates a heredoc file and writes input to it.
   Returns the filename of the generated heredoc. */
char	*generate_temp_heredoc(t_simple_cmds *cmd_tbl, char *stop_word,
		t_tools *shell)
{
	int	fd;

	cmd_tbl->hd_file_name = generate_temp_filename(cmd_tbl);
	fd = open(cmd_tbl->hd_file_name, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (fd == -1)
	{
		printf("%s%s\n", "SHELL", strerror(errno));
		return (NULL);
	}
	write_to_file(fd, stop_word, shell);
	shell->requires_expansion = 0;
	close(fd);
	return (cmd_tbl->hd_file_name);
}
