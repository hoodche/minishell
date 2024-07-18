/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 15:21:59 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 21:59:16 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief	extracts redirection >> from cmd->str filling its redir cmd->redir
 * 			struct .
 * 			- line 35 ft_atoi returns 0 if receives a non digit char
 * 			- line 38 corrects 0 returned by ft_atoi when it parsed a non digit 
 * 			- line 41 moves pointer back to char right after >> symbol
 * 			- line 43 moves until first char of the filename
 * 			- line 51 moves to char right after last char of the filename 
 * 					(or to next redirec symbol, or end of line)
 * 			- line 55 overwrites whole redirec substring (from 
 * 					start_redirec_substring to end_redirec_substring both 
 * 					included) in cmd->str with spaces for further split
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	char	*aux pointer to the char before first symbol of this 
 * 					redirection (>) in cmd->str
 * @param	int		j index of this redirection in the cmd->redir array 
 * @return	int		0 on error, 1 on success
 */
int	ext_red_append(t_cmd *cmd, char *aux, int j)
{
	char	*st_red_sbstr;
	char	*end_red_substr;
	char	*st_flname;

	cmd->redir[j].kind = APPEND;
	while (aux >= cmd->str && ft_isdigit(*aux))
		aux--;
	cmd->redir[j].fd = ft_atoi(++aux);
	if (*(cmd->redirec_ptr[j] - 1) < '0' || *(cmd->redirec_ptr[j] - 1) > '9')
		cmd->redir[j].fd = 1;
	st_red_sbstr = aux;
	aux = cmd->redirec_ptr[j] + 2;
	while (*aux && (*aux == ' ' || *aux == '\t'))
		aux++;
	if (!(*aux))
		return (mini_perror(UNEXPECTED_NL), g_status = 2, 0);
	st_flname = aux;
	while (*aux && (aux < cmd->redirec_ptr[j + 1] || !cmd->redirec_ptr[j + 1])
		&& ((*aux != ' ' && *aux != '\t') || is_quoted_in_cmd(cmd, aux)))
		aux++;
	end_red_substr = aux -1 ;
	cmd->redir[j].filename = extr_red_flnm_frm_cm(cmd, st_flname,
			end_red_substr, &(cmd->redir[j]));
	ft_memset(st_red_sbstr, ' ', end_red_substr - st_red_sbstr +1);
	return (1);
}

/**
 * @brief	extracts redirection << from cmd->str filling its redir cmd->redir
 * 			struct .
 *  * 		- line 89 ft_atoi returns 0 if receives a non digit char
 * 			- line 91 moves pointer back to char right after << symbol
 * 			- line 92 moves until first char of the filename
 * 			- line 97 moves to char right after last char of the filename 
 * 					(or to next redirec symbol, or end of line)
 * 			- line 103 overwrites whole redirec substring (from 
 * 					start_redirec_substring to end_redirec_substring both 
 * 					included) in cmd->str with spaces for further split
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	char	*aux pointer to the first symbol of this redirection (<) in 
 * 					cmd->str
 * @param	int		j index of this redirection in the cmd->redir array 
 * @return	int		0 on error, 1 on success
 */
int	ext_red_heredoc(t_cmd *cm, char *aux, int j)
{
	char	*st_red_substr;
	char	*end_red_sbstr;
	char	*st_flname;

	cm->redir[j].kind = HEREDOC;
	aux--;
	while (aux >= cm->str && ft_isdigit(*aux))
		aux--;
	cm->redir[j].fd = ft_atoi(++aux);
	st_red_substr = aux;
	aux = cm->redirec_ptr[j] + 2;
	while (*aux && (*aux == ' ' || *aux == '\t'))
		aux++;
	if (!(*aux))
		return (mini_perror(UNEXPECTED_NL), g_status = 2, 0);
	st_flname = aux;
	while (*aux && (aux < cm->redirec_ptr[j + 1] || !cm->redirec_ptr[j + 1])
		&& ((*aux != ' ' && *aux != '\t') || is_quoted_in_cmd(cm, aux)))
		aux++;
	end_red_sbstr = aux -1 ;
	cm->redir[j].delimeter = extr_red_flnm_frm_cm(cm,
			st_flname, end_red_sbstr, &(cm->redir[j]));
	return (ft_memset(st_red_substr, ' ', end_red_sbstr - st_red_substr +1), 1);
}

/**
 * @brief	extracts redirection < from cmd->str filling its redir cmd->redir
 * 			struct .
 *  *  * 	- line 135 ft_atoi returns 0 if receives a non digit char
 * 			- line 137 moves pointer back to char right after < symbol
 * 			- line 138 moves until first char of the filename
 * 			- line 143 moves to char right after last char of the filename 
 * 					(or to next redirec symbol, or end of line)
 * 			- line 147 overwrites whole redirec substring (from 
 * 					start_redirec_substring to end_redirec_substring both 
 * 					included) in cmd->str with spaces for further split
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	char	*aux pointer to the first symbol of this redirection (<) in 
 * 					cmd->str
 * @param	int		j index of this redirection in the cmd->redir array 
 * @return	int		0 on error, 1 on success
 */
int	ext_red_in(t_cmd *cmd, char *aux, int j)
{
	char	*start_redirec_substring;
	char	*end_redirec_substring;
	char	*start_filename;

	cmd->redir[j].kind = IN;
	aux--;
	while (aux >= cmd->str && ft_isdigit(*aux))
		aux--;
	cmd->redir[j].fd = ft_atoi(++aux);
	start_redirec_substring = aux;
	aux = cmd->redirec_ptr[j] + 1;
	while (*aux && (*aux == ' ' || *aux == '\t'))
		aux++;
	if (!(*aux))
		return (mini_perror(UNEXPECTED_NL), g_status = 2, 0);
	start_filename = aux;
	while (*aux && (aux < cmd->redirec_ptr[j + 1] || !cmd->redirec_ptr[j + 1])
		&& ((*aux != ' ' && *aux != '\t') || is_quoted_in_cmd(cmd, aux)))
		aux++;
	end_redirec_substring = aux -1 ;
	cmd->redir[j].filename = extr_red_flnm_frm_cm(cmd, start_filename,
			end_redirec_substring, &(cmd->redir[j]));
	ft_memset(start_redirec_substring, ' ',
		end_redirec_substring - start_redirec_substring + 1);
	return (1);
}

/**
 * @brief	extracts redirection > from cmd->str filling its redir cmd->redir
 * 			struct .
 *  *  *  * - line 179 ft_atoi returns 0 if receives a non digit char
 * 			- line 181 corrects 0 returned by ft_atoi when it parsed a non digit
 * 			- line 183 moves pointer back to char right after > symbol
 * 			- line 184 moves until first char of the filename
 * 			- line 190 moves to char right after last char of the filename 
 * 					(or to next redirec symbol, or end of line)
 * 			- line 194 overwrites whole redirec substring (from 
 * 					start_redirec_substring to end_redirec_substring both 
 * 					included) in cmd->str with spaces for further split
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	char	*aux pointer to the first symbol of this redirection (<) in 
 * 					cmd->str
 * @param	int		j index of this redirection in the cmd->redir array 
 * @return	int		0 on error, 1 on success
 */
int	ext_red_out(t_cmd *cmd, char *aux, int j)
{
	char	*st_red_sbstr;
	char	*end_red_sbstr;
	char	*st_flname;

	cmd->redir[j].kind = OUT;
	aux--;
	while (aux >= cmd->str && ft_isdigit(*aux))
		aux--;
	cmd->redir[j].fd = ft_atoi(++aux);
	if (*(cmd->redirec_ptr[j] - 1) < '0' || *(cmd->redirec_ptr[j] - 1) > '9')
		cmd->redir[j].fd = 1;
	st_red_sbstr = aux;
	aux = cmd->redirec_ptr[j] + 1;
	while (*aux && (*aux == ' ' || *aux == '\t'))
		aux++;
	if (!(*aux))
		return (mini_perror(UNEXPECTED_NL), g_status = 2, 0);
	st_flname = aux;
	while (*aux && (aux < cmd->redirec_ptr[j + 1] || !cmd->redirec_ptr[j + 1])
		&& ((*aux != ' ' && *aux != '\t') || is_quoted_in_cmd(cmd, aux)))
		aux++;
	end_red_sbstr = aux -1 ;
	cmd->redir[j].filename = extr_red_flnm_frm_cm(cmd, st_flname, end_red_sbstr,
			&(cmd->redir[j]));
	return (ft_memset(st_red_sbstr, ' ', end_red_sbstr - st_red_sbstr +1), 1);
}

/**
 * @brief	extracts redirections from each cmd->str filling its redir 
 * 			cmd->redir struct .
 * @param	t_read_input	*input struct with cmds
 * @return	int 0 on error, 1 on success
 */
int	extract_redirs_from_cmds(t_read_input *input)
{
	int		i;
	int		j;
	char	*aux;

	i = -1;
	while (++i < input->cmd_count)
	{
		j = -1;
		while (input->cmds[i].redirec_ptr[++j])
		{
			aux = input->cmds[i].redirec_ptr[j];
			if (*aux == '>' && *(aux + 1) == '>'
				&& (!(ext_red_append(&(input->cmds[i]), aux -1, j))))
				return (0);
			else if (*aux == '<' && *(aux + 1) == '<'
				&& (!(ext_red_heredoc(&(input->cmds[i]), aux, j))))
				return (0);
			else if (*aux == '<' && (!(ext_red_in(&(input->cmds[i]), aux, j))))
				return (0);
			else if (*aux == '>' && (!(ext_red_out(&(input->cmds[i]), aux, j))))
				return (0);
		}
	}
	return (1);
}
