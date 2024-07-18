/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptr_recalculation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:59:39 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 21:59:16 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief	fills the cmd->ruling_dbl_quotes array with pointers to cmd->str
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	int		cmd_idx index of the command in the input->cmds array
 * @param	*int	pointer to counter of ruling_dbl_quotes in the 
 * 					input->ruling_dbl_quotes array
 */
void	reclculate_dbl_quote_ptrs(t_read_input *input, int cmd_idx, int *i_dbl)
{
	int	m;

	m = 0;
	while (input->ruling_dbl_quotes[*i_dbl]
		&& (input->ruling_dbl_quotes[*i_dbl] < input->pipes[cmd_idx + 1]))
	{
		input->cmds[cmd_idx].ruling_dbl_quotes[m] = input->cmds[cmd_idx].str
			+ (input->ruling_dbl_quotes[*i_dbl] - input->pipes[cmd_idx]);
		(*i_dbl)++;
		m++;
		input->cmds[cmd_idx].ruling_dbl_quotes[m] = input->cmds[cmd_idx].str
			+ (input->ruling_dbl_quotes[*i_dbl] - input->pipes[cmd_idx]);
		(*i_dbl)++;
		m++;
	}
}

/**
 * @brief	fills the cmd->ruling_sgl_quotes array with pointers to cmd->str
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	int		cmd_idx index of the command in the input->cmds array
 * @param	*int	pointer to counter of ruling_sgl_quotes in the 
 * 					input->ruling_sgl_quotes array
 */
void	reclculate_sgl_quote_ptrs(t_read_input *input, int cmd_idx, int *i_sgl)
{
	int	m;

	m = 0;
	while (input->ruling_sgl_quotes[*i_sgl]
		&& (input->ruling_sgl_quotes[*i_sgl] < input->pipes[cmd_idx + 1]))
	{
		input->cmds[cmd_idx].ruling_sgl_quotes[m] = input->cmds[cmd_idx].str
			+ (input->ruling_sgl_quotes[*i_sgl] - input->pipes[cmd_idx]);
		(*i_sgl)++;
		m++;
		input->cmds[cmd_idx].ruling_sgl_quotes[m] = input->cmds[cmd_idx].str
			+ (input->ruling_sgl_quotes[*i_sgl] - input->pipes[cmd_idx]);
		(*i_sgl)++;
		m++;
	}
}

/**
 * @brief	fills the cmd->redirec_ptr array with pointers to cmd->str
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	int		cmd_idx index of the command being parsed in 
 * 			input->cmds array
 * @param	*int	pointer to counter of redirs in the input->redirec_ptr array
 */
void	recalculate_redirs_ptrs(t_read_input *input, int cmd_idx, int *i_red)
{
	int	m;

	m = 0;
	while (input->redirec_ptr[*i_red]
		&& (input->redirec_ptr[*i_red] < input->pipes[cmd_idx + 1]))
	{
		input->cmds[cmd_idx].redirec_ptr[m] = input->cmds[cmd_idx].str
			+ (input->redirec_ptr[*i_red] - input->pipes[cmd_idx]);
		(*i_red)++;
		m++;
	}
}

/**
 * @brief	fills the cmd->ruling_dbl_quotes array with pointers to cmd->str
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	int		cmd_idx index of the LASTcommand in the input->cmds array
 * @param	*int	pointer to counter of ruling_dbl_quotes in the 
 * 					input->ruling_dbl_quotes array
 */
void	recalc_dbl_qote_ptrs_last(t_read_input *input, int cmd_idx, int *i_dbl)
{
	int	m;

	m = 0;
	while (input->ruling_dbl_quotes[*i_dbl])
	{
		input->cmds[cmd_idx].ruling_dbl_quotes[m] = input->cmds[cmd_idx].str
			+ (input->ruling_dbl_quotes[*i_dbl] - input->pipes[cmd_idx]);
		(*i_dbl)++;
		m++;
		input->cmds[cmd_idx].ruling_dbl_quotes[m] = input->cmds[cmd_idx].str
			+ (input->ruling_dbl_quotes[*i_dbl] - input->pipes[cmd_idx]);
		(*i_dbl)++;
		m++;
	}
}

/**
 * @brief	fills the cmd->ruling_dbl_quotes array with pointers to cmd->str
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	int		cmd_idx index of the LASTcommand in the input->cmds array
 * @param	*int	pointer to counter of ruling_dbl_quotes in the 
 * 					input->ruling_dbl_quotes array
 */
void	recalc_sgl_qote_ptrs_last(t_read_input *input, int cmd_idx, int *i_sgl)
{
	int	m;

	m = 0;
	while (input->ruling_sgl_quotes[*i_sgl])
	{
		input->cmds[cmd_idx].ruling_sgl_quotes[m] = input->cmds[cmd_idx].str
			+ (input->ruling_sgl_quotes[*i_sgl] - input->pipes[cmd_idx]);
		(*i_sgl)++;
		m++;
		input->cmds[cmd_idx].ruling_sgl_quotes[m] = input->cmds[cmd_idx].str
			+ (input->ruling_sgl_quotes[*i_sgl] - input->pipes[cmd_idx]);
		(*i_sgl)++;
		m++;
	}
}
