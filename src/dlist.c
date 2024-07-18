/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 21:28:14 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/13 21:31:28 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_dlist	*ft_dlistnew(char *key, char *value)
{
	t_dlist	*l;

	l = (t_dlist *) malloc(sizeof(t_dlist));
	if (l == NULL)
		return (NULL);
	l->backup = NULL;
	l->value = value;
	l->key = key;
	l->status = ORIGINAL;
	l->prev = NULL;
	l->next = NULL;
	return (l);
}

void	ft_dlistadd_front(t_dlist **l, t_dlist *new)
{
	t_dlist	*next;

	if (l == NULL)
		return ;
	next = *l;
	new->next = next;
	if (next)
		next->prev = new;
	*l = new;
}

t_dlist	*ft_dlistlast(t_dlist *l)
{
	while (l && l->next)
		l = l->next;
	return (l);
}

void	ft_dlistadd_back(t_dlist **l, t_dlist *new)
{
	t_dlist	*last;

	if (l == NULL)
		return ;
	last = ft_dlistlast(*l);
	if (last)
	{
		last->next = new;
		new->prev = last;
	}
	else
		*l = new;
}

void	ft_dlistclear(t_dlist **l)
{
	t_dlist	*next;
	t_dlist	*first;

	if (l == NULL)
		return ;
	first = *l;
	while (first)
	{
		next = first->next;
		if (first->backup)
			free(first->backup);
		if (first->key)
			free(first->key);
		if (first->value)
			free(first->value);
		free(first);
		first = next;
	}
	*l = NULL;
}
