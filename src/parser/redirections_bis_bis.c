/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_bis_bis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:41:19 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 21:59:16 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

/**
 * @brief	checks if a character (used with '$') is part of a heredoc delimeter
 *          (is witin the word following a <<)
 * @param	t_cmd *cmd struct whose cmd->str is being parsed
 * @param	char *let char to be checked
 * @return	int 1 if char is part of a heredoc delimeter, 0 if not
 */
int	is_in_heredoc_delimeter(t_cmd *cmd, char *let)
{
	while ((*let != ' ' && *let != '\t' && *let != '<')
		|| is_quoted_in_cmd(cmd, let))
		let--;
	if (*let == '<' && *(let - 1) == '<')
		return (1);
	else
	{
		while (*let && (*let == ' ' || *let == '\t'))
			let--;
		if (*let == '<' && *(let - 1) == '<')
			return (1);
	}
	return (0);
}
