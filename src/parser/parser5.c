/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:47:46 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:32:48 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Checks if a character is a visible character.
   Returns 1 if the character is visible, otherwise returns 0. */
int	is_visible_char(char c)
{
	int	i;

	i = is_special_ascii(c);
	if (i == 0)
		return (1);
	else
		return (0);
}

/* Creates a new token with the given content and type.
   Returns a pointer to the new token. */
t_node	*create_token(char *content, t_class type)
{
	t_node	*new_token;

	new_token = (t_node *)smalloc(sizeof(t_node));
	if (new_token == NULL)
	{
		return (NULL);
	}
	new_token->data = content;
	new_token->class = type;
	new_token->prev = NULL;
	new_token->next = NULL;
	return (new_token);
}

/* Creates a new token with the given string and type.
   Returns a pointer to the new token. */
t_node	*create_new_token(char *str, t_class type)
{
	return (create_token(str, type));
}

/* Inserts a new token into a linked list of tokens.
   Returns the head of the token list. */
t_node	*insert_token(t_node *head, char *str, t_class type)
{
	t_node	*tail;
	t_node	*new_token;

	if (head == NULL)
	{
		return (create_new_token(str, type));
	}
	tail = head;
	while (tail->next != NULL)
	{
		tail = tail->next;
	}
	new_token = create_token(str, type);
	if (new_token == NULL)
	{
		return (head);
	}
	tail->next = new_token;
	new_token->prev = tail;
	return (head);
}

/* Prints an error message. */
void	raise_error(char *msg)
{
	printf("%s\n", msg);
}
