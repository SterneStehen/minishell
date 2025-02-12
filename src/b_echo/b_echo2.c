/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:16:13 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:25:03 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	analyze_and_handle_quotes(char *input, int *ctx)
{
	if (input[ctx[0]] == 34)
	{
		if (!(ctx[2]))
			ctx[1] = !(ctx[1]);
		else
			safe_write(1, &input[ctx[0]], 1);
		ctx[0]++;
	}
	else if (input[ctx[0]] == 39)
	{
		if (!(ctx[1]))
			ctx[2] = !(ctx[2]);
		else
			safe_write(1, &input[ctx[0]], 1);
		ctx[0]++;
	}
}

int	print_without_quotes(char *input)
{
	int	ctx[4];

	ctx[0] = 0;
	ctx[1] = 0;
	ctx[2] = 0;
	ctx[3] = 0;
	analyze_quotes(input, &ctx[3]);
	while (input[ctx[0]] != '\0')
	{
		analyze_and_handle_quotes(input, ctx);
		if (input[ctx[0]] == '\0')
			break ;
		while ((input[ctx[0]] == 32 && input[ctx[0] + 1] == 32 && !ctx[3])
			|| input[ctx[0]] == 92)
			ctx[0]++;
		if (input[ctx[0]] == 92)
			handle_escaped_chars(input, &ctx[0]);
		else
			print_non_quote_chars2(input, ctx[2], ctx[1], &ctx[0]);
	}
	return (0);
}

// Функция для обработки одного аргумента
void	process_single_arg(char *arg, int *current_is_space_filled)
{
	print_without_quotes(arg);
	*current_is_space_filled = is_space_filled(arg);
}

// Функция для обработки аргументов
void	process_args(char **arguments, int startIndex)
{
	int	*p_current_space;
	int	*p_next_space;

	p_current_space = (int *)malloc(sizeof(int));
	p_next_space = (int *)malloc(sizeof(int));
	while (arguments[startIndex] != NULL)
	{
		process_single_arg(arguments[startIndex], p_current_space);
		if (arguments[startIndex + 1] != NULL)
		{
			*p_next_space = is_space_filled(arguments[startIndex + 1]);
		}
		if (arguments[startIndex + 1] != NULL && (!(*p_current_space)
				|| (*p_next_space)))
		{
			write(1, " ", 1);
		}
		startIndex++;
	}
	free(p_current_space);
	free(p_next_space);
}

// Функция для обработки флага -n
int	handle_flag_n(char **args, int start)
{
	if (strncmp(args[start], "-n", 2) == 0)
	{
		if (is_flag_valid(args[start]) == 1)
		{
			write(1, "", 1);
			return (1);
		}
	}
	return (0);
}
