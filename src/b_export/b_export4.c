/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:20:50 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:25:45 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
// Main function to get environment content
char	*allocate_empty_content(void)
{
	char	*result_content;

	result_content = smalloc(sizeof(char) * 2);
	if (result_content == NULL)
	{
		return (NULL);
	}
	result_content[0] = ' ';
	result_content[1] = '\0';
	return (result_content);
}

char	*allocate_full_content(int content_size)
{
	char	*result_content;

	result_content = smalloc(sizeof(char) * (content_size + 1));
	if (result_content == NULL)
	{
		return (NULL);
	}
	return (result_content);
}

char	*extract_env_data(char *input_str, char *separator)
{
	char	*result_content;
	size_t	sep_len;
	int		len_diff;
	int		content_size;

	sep_len = strlen(separator);
	if (sep_len == strlen(input_str) - 1)
		result_content = allocate_empty_content();
	else if (sep_len == strlen(input_str))
		result_content = NULL;
	else
	{
		len_diff = calc_lengths(input_str, separator);
		content_size = len_diff - 1;
		result_content = allocate_full_content(content_size);
		if (result_content == NULL)
			return (NULL);
		copy_content(input_str, result_content, sep_len + 1);
	}
	return (result_content);
}

void	insert_new_variable(t_tools *tools, char *source)
{
	t_environment	*new_var;
	t_environment	*current_env;

	new_var = create_envairm(source);
	if (tools->envair == NULL)
	{
		tools->envair = new_var;
	}
	else
	{
		current_env = tools->envair;
		while (current_env->next != NULL)
		{
			current_env = current_env->next;
		}
		current_env->next = new_var;
	}
}

int	compare_var_title(char *title, char *var)
{
	int	j;

	j = 0;
	while (title[j] || var[j])
	{
		if (title[j] != var[j])
			return (0);
		j++;
	}
	return (!title[j] && !var[j]);
}
