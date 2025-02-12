/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoreron <7353718@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:43:15 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/07/09 20:34:24 by smoreron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Allocates memory of the specified size.
   Prints an error message and returns NULL if the allocation fails. */
void	*smalloc(size_t size)
{
	void		*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	return (ptr);
}
