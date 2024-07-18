/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 09:35:21 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 11:57:59 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
