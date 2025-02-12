/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:16:13 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:25:13 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_space_filled(char *arg)
{
	int	idx;

	idx = 0;
	while (arg[idx] != '\0')
	{
		if (arg[idx] == 34 || arg[idx] == 39 || arg[idx] == 32)
		{
			idx++;
		}
		else
		{
			return (0);
		}
	}
	return (1);
}

void	safe_write(int fd, const void *buf, size_t count)
{
	if (write(fd, buf, count) == -1)
	{
		printf("Write error\n");
		exit(EXIT_FAILURE);
	}
}

// Функция для обработки экранированных символов
void	handle_escaped_chars(char *str, int *index)
{
	int	count;
	int	i;
	int	to_write;

	count = 0;
	i = *index;
	while (str[i] == 92)
	{
		count++;
		i++;
	}
	*index = i;
	to_write = (count / 2);
	if (count % 2 != 0)
	{
		to_write++;
	}
	while (to_write > 0)
	{
		safe_write(1, "\\", 1);
		to_write--;
	}
}

// Функция для печати символов, не являющихся кавычками
void	print_non_quote_chars2(char *str, int single_quote_detected,
		int double_quote_detected, int *index)
{
	if (str[*index] != 34 && str[*index] != 39)
	{
		safe_write(1, &str[*index], 1);
	}
	else if (str[*index] == 34 && single_quote_detected)
	{
		safe_write(1, &str[*index], 1);
	}
	else if (str[*index] == 39 && double_quote_detected)
	{
		safe_write(1, &str[*index], 1);
	}
	(*index)++;
}

// Функция для анализа наличия кавычек в строке
int	analyze_quotes(char *str, int *quote_found)
{
	int	j;

	j = 0;
	while (str[j] != '\0')
	{
		if (str[j] == 34 || str[j] == 39)
		{
			*quote_found = 1;
			break ;
		}
		j++;
	}
	return (0);
}
