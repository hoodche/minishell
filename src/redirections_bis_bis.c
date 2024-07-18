/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_bis_bis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:41:19 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/07 17:42:07 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief	initializes parsing index counters for single and double quotes
 * @param	int		*si index of ruling_sgl_quotes array where parsing is at
 * @param	int		*di index of ruling_dbl_quotes array where parsing is at
 */
void	init_index_quotes_counters(int *si, int *di)
{
	*si = 0;
	*di = 0;
}
