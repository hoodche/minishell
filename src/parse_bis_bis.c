/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bis_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:53:39 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/13 22:47:51 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief	if quotes are found while parsing cmd_str, gets the substring before
 * 			the quote char and appends it to the word being built
 * @param	char **cmd_str being parsed
 * @param	char *chunk_begin pointer to the beginning of the chunk being built
 * @param	char **word pointer to the accumulated word being built
 */
void	append_substr_befor_quote(char *cmd_str, char *chunk_begin, char **word)
{
	char	*word_aux;
	char	*chunk;

	word_aux = ft_substr(chunk_begin, 0, cmd_str - chunk_begin);
	if (word_aux)
	{
		chunk = ft_strjoin(*word, word_aux);
		free(word_aux);
		free(*word);
		*word = chunk;
	}
}

/**
 * @brief	if single quotes are found while parsing cmd_str, gets the substring
 * 			between quotes and appends it to the word being built
 * @param	char **cmd_str being parsed
 * @param	t_cmd *cmd struct with ruling_quotes arrays
 * @param	int *i_sgl index of ruling_sgl_quotes array where parsing is at
 * @param	char **word pointer to the accumulated word being built
 */
void	append_substr_bet_sgl_quot(char **cmd_str,
			t_cmd *cmd, int *i_sgl, char **word)
{
	char	*word_aux;
	char	*chunk;

	word_aux = ft_substr(*cmd_str + 1, 0, cmd->ruling_sgl_quotes[*i_sgl + 1]
			- (*cmd_str + 1));
	chunk = ft_strjoin(*word, word_aux);
	free(word_aux);
	free(*word);
	*word = chunk;
	*cmd_str = cmd->ruling_sgl_quotes[*i_sgl + 1] + 1;
	*i_sgl += 2;
}

/**
 * @brief	if double quotes are found while parsing cmd_str, gets the substring
 * 			between quotes and appends it to the word being built
 * @param	char **cmd_str being parsed
 * @param	t_cmd *cmd struct with ruling_quotes arrays
 * @param	int *i_dbl index of ruling_sgl_quotes array where parsing is at
 * @param	char **word pointer to the accumulated word being built
 */
void	append_substr_bet_dbl_quot(char **cmd_str,
		t_cmd *cmd, int *i_dbl, char **word)
{
	char	*word_aux;
	char	*chunk;

	word_aux = ft_substr(*cmd_str + 1, 0, cmd->ruling_dbl_quotes[*i_dbl + 1]
			- (*cmd_str + 1));
	chunk = ft_strjoin(*word, word_aux);
	free(word_aux);
	free(*word);
	*word = chunk;
	*cmd_str = cmd->ruling_dbl_quotes[*i_dbl + 1] + 1;
	*i_dbl += 2;
}

/**
 * @brief	compose the final word when parsing cmd_str is over
 * @param	char **cmd_str being parsed
 * @param	char *chunk_begin pointer to the beginning of the chunk being built
 * @param	char **word pointer to the accumulated word being built
 */
char	*append_final_word(char *cmd_str, char *chunk_begin, char *word)
{
	char	*word_aux;
	char	*ret;

	word_aux = ft_substr(chunk_begin, 0, cmd_str - chunk_begin);
	ret = ft_strjoin(word, word_aux);
	free(word_aux);
	free(word);
	return (ret);
}

/**
 * @brief	extracts word from cmd_str when a non space char was parsed
 * @param	char **cmd_str being parsed
 * @param	t_cmd *cmd struct with ruling_quotes arrays
 * @param	int *i_sgl index of ruling_sgl_quotes array where parsing is at
 * 			this moment
 * @param	int *i_dbl index of ruling_dbl_quotes array where parsing is at
 * 			this moment
 */

char	*get_word(char **cmd_str, t_cmd *cmd, int *i_sgl, int *i_dbl)
{
	char	*chunk_begin;
	char	*word;

	word = ft_strdup("");
	chunk_begin = *cmd_str;
	while (**cmd_str && **cmd_str != ' ' && **cmd_str != '\t')
	{
		if (*cmd_str == cmd->ruling_sgl_quotes[*i_sgl])
		{
			append_substr_befor_quote(*cmd_str, chunk_begin, &word);
			append_substr_bet_sgl_quot(cmd_str, cmd, i_sgl, &word);
			chunk_begin = *cmd_str;
		}
		else if (*cmd_str == cmd->ruling_dbl_quotes[*i_dbl])
		{
			append_substr_befor_quote(*cmd_str, chunk_begin, &word);
			append_substr_bet_dbl_quot(cmd_str, cmd, i_dbl, &word);
			chunk_begin = *cmd_str;
		}
		else
			(*cmd_str)++;
	}
	return (append_final_word(*cmd_str, chunk_begin, word));
}
