/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:54:05 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 21:59:16 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief	checks if a string (inpput typed at prompt) has balanced quotes
 * @param	char *typed
 * @return	int 1 if quotes are balanced, 0 if not
 */
int	are_input_quotes_balanced(char *typed)
{
	while (*typed)
	{
		if (*typed == '\"')
		{
			typed++;
			while (*typed && *typed != '\"')
				typed++;
			if (!*typed)
				return (mini_perror(UNB_DOUB_QUOT), g_status = 127, 0);
		}
		if (*typed == '\'')
		{
			typed++;
			while (*typed && *typed != '\'')
				typed++;
			if (!*typed)
				return (mini_perror(UNB_SING_QUOT), g_status = 127, 0);
		}
		typed++;
	}
	return (1);
}

/**
 * @brief	fills ruling_quotes arrays in t_read_input with the indexes of the 
 * 			opening and closing quotes
 * @param	t_read_input *read_input
 */
void	locate_input_quotes(t_read_input *read_input)
{
	int		dbl_quotes_index;
	int		sgl_quotes_index;
	char	*aux;

	aux = read_input->typed;
	dbl_quotes_index = 0;
	sgl_quotes_index = 0;
	while (*aux)
	{
		if (*aux == '\"')
		{
			read_input->ruling_dbl_quotes[dbl_quotes_index++] = aux++;
			while (*aux != '\"')
				aux++;
			read_input->ruling_dbl_quotes[dbl_quotes_index++] = aux;
		}
		if (*aux == '\'')
		{
			read_input->ruling_sgl_quotes[sgl_quotes_index++] = aux++;
			while (*aux != '\'')
				aux++;
			read_input->ruling_sgl_quotes[sgl_quotes_index++] = aux;
		}
		aux++;
	}
}

/**
 * @brief	checks if a char is between quotes
 * @param	t_read_input *read_input struct with ruling_quotes arrays
 * @param	char *let char to be checked
 * @return	int 1 if char is between quotes, 0 if not
 */
int	is_quoted_in_input(t_read_input *input, char *let)
{
	int	i;

	i = 0;
	while (input->ruling_dbl_quotes[i])
	{
		if ((let > input->ruling_dbl_quotes[i])
			&& (let < input->ruling_dbl_quotes[i + 1]))
			return (1);
		i += 2;
	}
	i = 0;
	while (input->ruling_sgl_quotes[i])
	{
		if ((let > input->ruling_sgl_quotes[i])
			&& (let < input->ruling_sgl_quotes[i + 1]))
			return (1);
		i += 2;
	}
	return (0);
}

/**
 * @brief	checks if a char is between quotes
 * @param	t_cmd *cmd struct with ruling_quotes arrays
 * @param	char *let char to be checked
 * @return	int 1 if char is between quotes, 0 if not
 */
int	is_quoted_in_cmd(t_cmd *cmd, char *let)
{
	int	i;

	i = 0;
	while (cmd->ruling_dbl_quotes[i])
	{
		if ((let > cmd->ruling_dbl_quotes[i])
			&& (let < cmd->ruling_dbl_quotes[i + 1]))
			return (1);
		i += 2;
	}
	i = 0;
	while (cmd->ruling_sgl_quotes[i])
	{
		if ((let > cmd->ruling_sgl_quotes[i])
			&& (let < cmd->ruling_sgl_quotes[i + 1]))
			return (1);
		i += 2;
	}
	return (0);
}

/**
 * @brief	checks if a char is between single quotes in a cmd
 * @param	t_cmd *cmd struct with ruling_quotes arrays
 * @param	char *let char to be checked
 * @return	int 1 if char is between quotes, 0 if not
 */
int	is_sgl_quoted_in_cmd(t_cmd *cmd, char *let)
{
	int	i;

	i = 0;
	while (cmd->ruling_sgl_quotes[i])
	{
		if ((let > cmd->ruling_sgl_quotes[i])
			&& (let < cmd->ruling_sgl_quotes[i + 1]))
			return (1);
		i += 2;
	}
	return (0);
}
