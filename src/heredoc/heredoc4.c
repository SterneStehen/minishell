/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:40:53 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 19:21:52 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Finds the last heredoc token in the redirections list.
   Returns a pointer to the heredoc token if found, otherwise NULL. */
t_node	*locate_heredoc_token(t_node *redirs_list)
{
	t_node	*heredoc_token;

	heredoc_token = NULL;
	while (redirs_list != NULL)
	{
		if (redirs_list->class == LESS_LESS)
			heredoc_token = redirs_list;
		redirs_list = redirs_list->next;
	}
	return (heredoc_token);
}
