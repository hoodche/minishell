/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_bis_bis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:41:19 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/22 16:52:55 by igcastil         ###   ########.fr       */
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
	while ((let >= cmd->str && *let != ' ' && *let != '\t' && *let != '<')
		|| is_quoted_in_cmd(cmd, let))
		let--;
	if (let > cmd->str && *let == '<' && *(let - 1) == '<')
		return (1);
	else
	{
		while (let >= cmd->str && *let && (*let == ' ' || *let == '\t'))
			let--;
		if (let > cmd->str && *let == '<' && *(let - 1) == '<')
			return (1);
	}
	return (0);
}

/**
 * @brief	parses a comand string,right after a redirection sign (> or <) 
 * 			that is a whiitespace or tabs checking for errors
 * @param	char	*aux pointer to twhiitespace or tabs right after a 
 * 			redirection sign
 * @return	int 0 on error, 1 on success
 */
int	blanks_between_redirections(char *aux)
{
	while (*aux == ' ' || *aux == '\t')
		aux++;
	if (*aux == '<')
		return (mini_perror(UNEXPECTED_RED_IN), g_status = 2, 0);
	else if (*aux == '>')
		return (mini_perror(UNEXPECTED_RED_OUT), g_status = 2, 0);
	return (1);
}

/**
 * @brief	parses a comand string,right after a >>> redirection sign
 * @param	char	*aux pointer char right after the 3rd redirection sign
 * @return	0 (always error, since minishell does not accept here strings >>>)
 */
int	check_here_string(char *aux)
{
	while (*aux == ' ' || *aux == '\t')
		aux++;
	if (*aux == '<')
		return (mini_perror(UNEXPECTED_RED_IN), g_status = 2, 0);
	else if (*aux == '>')
		return (mini_perror(UNEXPECTED_RED_OUT), g_status = 2, 0);
	else if (*aux == 0)
		return (mini_perror(UNEXPECTED_NL), g_status = 2, 0);
	else
		return (mini_perror(HERE_STRING), g_status = 2, 0);
}
/**
 * @brief	parses a comand string,right after a redirection sign (> or <) 
 * 			checking for errors
 * @param	char	*aux pointer to the redirection sign
 * @return	int 0 on error, 1 on success
 */
int	redirec_sign_errors(char *au)
{
	if (*au == '>' && *(au + 1) == '<')
		return (mini_perror(UNEXPECTED_RED_IN), g_status = 2, 0);
	if ((*au == '>' || *au == '<') && (*(au + 1) == ' ' || *(au + 1) == '\t'))
	{
		if (blanks_between_redirections(au + 1) == 0)
			return (0);
	}
	if (*au == '<' && *(au + 1) == '<' && *(au + 2) == '>')
		return (mini_perror(UNEXPECTED_RED_OUT), g_status = 2, 0);
	if (*au == '>' && *(au + 1) == '>' && (*(au + 2) == '<'))
		return (mini_perror(UNEXPECTED_RED_IN), g_status = 2, 0);
	if (*au == '>' && *(au + 1) == '>' && (*(au + 2) == '>'))
		return (mini_perror(UNEXPECTED_RED_OUT), g_status = 2, 0);
	if (((*au == '<' && *(au + 1) == '<') || (*au == '>' && *(au + 1) == '>'))
		&& (*(au + 2) == ' ' || *(au + 2) == '\t'))
	{
		if (blanks_between_redirections(au + 2) == 0)
			return (0);
	}
	if (*au == '<' && *(au + 1) == '<' && *(au + 2) == '<')
		return(check_here_string(au + 3));
	return (1);
}
