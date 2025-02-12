/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:54:54 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:33:07 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Finds wrong redirections in the token list.
   Returns 1 if a wrong redirection is found, otherwise 0. */
int	find_invalid_redirection(t_node *start)
{
	t_node	*current;
	t_node	*possible_wrong;

	current = start;
	possible_wrong = current;
	while (possible_wrong != NULL)
	{
		if (!is_redirection_token(possible_wrong))
		{
			possible_wrong = possible_wrong->next;
			continue ;
		}
		if (!(is_redirection_token(possible_wrong)
				&& possible_wrong->next != NULL
				&& !is_redirection_token(possible_wrong->next)))
		{
			break ;
		}
		possible_wrong = possible_wrong->next->next;
	}
	return (possible_wrong != NULL);
}
