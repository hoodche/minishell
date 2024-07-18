/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:47:03 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 21:59:16 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief	moves forward pointer to cmd_str when a space was parsed
 *			If pointer points to an overwritten single or double quote skips 
 			right after the closing quote
 * @param	char **cmd_str being parsed
 * @param	t_cmd *cmd struct with ruling_quotes arrays
 * @param	int *i_sgl index of ruling_sgl_quotes array where parsing is at
 * 			this moment
 * @param	int *i_dbl index of ruling_dbl_quotes array where parsing is at
 * 			this moment
 */
void	move_ptr_forward(char **cmd_str, t_cmd *cmd, int *i_sgl, int *i_dbl)
{
	if (*cmd_str == cmd->ruling_sgl_quotes[*i_sgl])
	{
		*cmd_str = cmd->ruling_sgl_quotes[*i_sgl + 1] + 1;
		*i_sgl += 2;
	}
	else if (*cmd_str == cmd->ruling_dbl_quotes[*i_dbl])
	{
		*cmd_str = cmd->ruling_dbl_quotes[*i_dbl + 1] + 1;
		*i_dbl += 2;
	}
	else
		(*cmd_str)++;
}

/**
 * @brief	parses each cmd->str splitting by ' ' and quotes to get final
 * 			char ** that will be passed to execve().
 * 			- line 65 pointer points to beginning of a word
 * 			- line 70 pointer points to a space
 * @param	t_read_input	*input struct with cmds
 */

void	get_cmds_2_be_executed(t_read_input *input)
{
	int		i;
	int		i_sgl;
	int		i_dbl;
	int		word_count;
	char	*aux;

	i = -1;
	while (++i < input->cmd_count)
	{
		aux = input->cmds[i].str;
		i_sgl = 0;
		i_dbl = 0;
		word_count = 0;
		while (*aux)
		{
			if (*aux != ' ' && *aux != '\t')
				input->cmds[i].argv_for_execve[word_count++]
					= get_word(&aux, &input->cmds[i], &i_sgl, &i_dbl);
			else
				move_ptr_forward(&aux, &input->cmds[i], &i_sgl, &i_dbl);
		}
	}
}
