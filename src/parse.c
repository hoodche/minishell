/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 12:36:30 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/17 13:07:06 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief	gets pointers to redirection signs in the input string, and stores
 * 			them in struct array input->redirec_ptr[i]
 * @param	t_read_input	*input struct with input string to be trimmed
 */
static	void	get_redirections(t_read_input *input)
{
	int		i;
	char	*aux;

	i = 0;
	aux = input->typed;
	while (*aux)
	{
		if ((*aux == '<' || *aux == '>') && !is_quoted_in_input(input, aux))
		{
			input->redirec_ptr[i++] = aux;
			if ((*aux == '<' && *(aux + 1) == '<')
				|| (*aux == '>' && *(aux + 1) == '>'))
				aux += 2;
			else
				aux++;
		}
		else
			aux++;
	}
}

/**
 * @brief	fills each cmd in cmds array input->cmds[i] with a duplicate for the
 * 			 command string and quotes and redirec_ptr pointers point to this
 * 			new duplicate (recalculating offsets from the ones gotten from the 
 * 			input string)
 * 				- code from line 70 on is to avoid reading out of bounds in
 * 				  input->pipes
 * 				- lines 67 and 72 get a duplicate of command substring from 
 * 				  input->typed (where "|" were already replaced by \0)
 * @param	t_read_input	*input struct with input string and date * 
 * @return	int 0 on error, 1 on success
 */
static	void	set_commands(t_read_input *input)
{
	int	i;
	int	i_dbl;
	int	i_sgl;
	int	i_red;
	int	m;

	i = -1;
	i_dbl = 0;
	i_sgl = 0;
	i_red = 0;
	while (++i < input->cmd_count - 1)
	{
		input->cmds[i].str = ft_strdup(input->pipes[i]);
		reclculate_dbl_quote_ptrs(input, i, &i_dbl);
		reclculate_sgl_quote_ptrs(input, i, &i_sgl);
		recalculate_redirs_ptrs(input, i, &i_red);
	}
	input->cmds[i].str = ft_strdup(input->pipes[i]);
	recalc_dbl_qote_ptrs_last(input, i, &i_dbl);
	recalc_sgl_qote_ptrs_last(input, i, &i_sgl);
	m = -1;
	while (input->redirec_ptr[i_red])
		input->cmds[i].redirec_ptr[++m] = input->cmds[i].str
			+ (input->redirec_ptr[i_red++] - input->pipes[i]);
}

/**
 * @brief	parses input string, fills prompt struct arrays with pointers to 
 * 			redirection signs, to the chars right after a valid | in the input
 * 			string and replaces those '|' with '\0' so that each command can be 
 * 			filled easily (in the array input->cmds[i]) with a duplicate of the
 * 			command (as well as all its corresponding quotes and redirec_ptr 
 * 			pointers recalculted to point to the new duplicate string) *
 * @param	t_read_input	*input struct to be parsed and filled
 * @return	int 0 on error, 1 on success
 */
static	int	split_pipeline(t_read_input *input)
{
	char	*aux;
	int		i;

	i = 0;
	get_redirections(input);
	aux = input->typed;
	while (*aux == ' ' || *aux == '\t')
		aux++;
	if (*aux == '|')
		return (mini_perror(PIPE_SIGN), g_status = 127, 0);
	input->pipes[i++] = aux;
	while (*aux)
	{
		if (*aux == '|' && !is_quoted_in_input(input, aux))
		{
			if (*(aux - 1) == '>')
				return (mini_perror(CLOBBER), g_status = 127, 0);
			else if (*(aux + 1) == '|')
				return (mini_perror(OR), g_status = 127, 0);
			input->pipes[i++] = aux + 1;
			*aux = '\0';
		}
		aux++;
	}
	return (input->cmd_count = i, set_commands(input), 1);
}

/**
 * @brief	parses input string splitting it into commands, expands $, extracts 
 * 			redirection blocks from every cmd->str
 * 			.
 * @param	t_read_input	*input struct to be parsed and filled
 * @return	int 0 on error, 1 on success
 */
int	parse(t_read_input *input)
{
	if (!split_pipeline(input))
		return (0);
	expand_dollar(input);
	if (!extract_redirs_from_cmds(input))
		return (0);
	get_cmds_2_be_executed(input);
	return (1);
}
