/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:29:47 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 21:59:16 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief	gets pointers to dbl quotes in a substring of cmd->str
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	char	**quotes array where found pointers are stored *
 * @param	int		*sgl_i index of ruling_sgl_quotes array where parsing is at
 * @param	int		*dbl_i index of ruling_dbl_quotes array where parsing is at
 * @return	int 1 only to save one line in calling function 
 * 				get_qts_in_cm_sbstr()
 */
int	gt_dbs(t_cmd *cmd, char **quotes, int *sgl_i, int *dbl_i)
{
	quotes[*sgl_i + *dbl_i] = cmd->ruling_dbl_quotes[*dbl_i];
	(*dbl_i)++;
	quotes[*sgl_i + *dbl_i] = cmd->ruling_dbl_quotes[*dbl_i];
	(*dbl_i)++;
	return (1);
}

/**
 * @brief	gets pointers to sgl quotes in a substring of cmd->str
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	char	**quotes array where found pointers are stored *
 * @param	int		*sgl_i index of ruling_sgl_quotes array where parsing is at
 * @param	int		*dbl_i index of ruling_dbl_quotes array where parsing is at
 * @return	int 1 only to save one line in calling function 
 * 				get_qts_in_cm_sbstr()
 */
int	gt_sgs(t_cmd *cmd, char **quotes, int *sgl_i, int *dbl_i)
{
	quotes[*sgl_i + *dbl_i] = cmd->ruling_sgl_quotes[*sgl_i];
	(*sgl_i)++;
	quotes[*sgl_i + *dbl_i] = cmd->ruling_sgl_quotes[*sgl_i];
	(*sgl_i)++;
	return (1);
}

/**
 * @brief	gets pointers to quotes in a substring of cmd->str
 * 				- line 70 cmd has dbl and single quotes, check both of them 
 * 						until only one kind remains to be extracted
 * 				- line 80 only one of the 2 following while loops will be
 * 						executed, if any
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	char	*start pointer to the start of parsing 
 * 					(used for the filename in a redirection in cmd->str)
 * @param	char	*end pointer to the end of parsing
 * 					(used for the filename in a redirection in cmd->str)
 * @param	char	*quotes[MAX_QUOTES] array where found pointers are stored * 
 */
void	get_quotes_in_cm_sbstr(t_cmd *c, char *s, char *e, char *q[MAX_QUOTES])
{
	int	si;
	int	di;

	init_index_quotes_counters(&si, &di);
	while (c->ruling_dbl_quotes[di] && c->ruling_sgl_quotes[si])
	{
		if (c->ruling_dbl_quotes[di] < c->ruling_sgl_quotes[si])
		{
			if (!(c->ruling_dbl_quotes[di] >= s
					&& c->ruling_dbl_quotes[di] <= e && gt_dbs(c, q, &si, &di)))
				return ;
		}
		else
		{
			if (!(c->ruling_sgl_quotes[si] >= s
					&& c->ruling_sgl_quotes[si] <= e && gt_sgs(c, q, &si, &di)))
				return ;
		}
	}
	while (c->ruling_dbl_quotes[di] && c->ruling_dbl_quotes[di] >= s
		&& c->ruling_dbl_quotes[di] <= e)
		gt_dbs(c, q, &si, &di);
	while (c->ruling_sgl_quotes[si] && c->ruling_sgl_quotes[si] >= s
		&& c->ruling_sgl_quotes[si] <= e)
		gt_sgs(c, q, &si, &di);
}

/**
 * @brief	gets the filename for a redirection while there are quotes in it
 *				- line 101 appends chunk before quote
 *				- line 105 appends chunk between quotes
 *				- line 109 moves pointer right after closing quote
 * @param	char	**start pointer to the start of the filename in cmd->str
 * @param	char	**quotes pointers to quotes in the filename in cmd->str
 * @return	char *	string with all substrings appended 
 */
char	*extract_redir_filename_while_quoted(char **quotes, char **start)
{
	char	*filename;
	char	*filename_aux;
	char	*aux;
	int		i;

	i = 0;
	filename = ft_strdup("");
	while (quotes[i])
	{
		filename_aux = ft_substr(*start, 0, quotes[i] - *start);
		aux = ft_strjoin(filename, filename_aux);
		free(filename);
		free(filename_aux);
		filename_aux = ft_substr(quotes[i] + 1, 0, quotes[i + 1]
				- quotes[i] - 1);
		filename = ft_strjoin(aux, filename_aux);
		free(aux);
		free(filename_aux);
		*start = quotes[i + 1] + 1;
		i += 2;
	}
	return (filename);
}

/**
 * @brief	composes the filename for a redirection received betwen 2 (char *)
 * 			pointers to the cmd->str, checking if there are quotes within that
 * 			 range
 *				- line 124 filename after redirec symbol has no quotes
 * @param	t_cmd	*pointer to the command struct (with quotes arrays)
 * @param	char	*start pointer to the start of the filename in cmd->str
 * @param	char	*end pointer to the end of the filename in cmd->str
 * @return	char *	pointer to the filename string
 */
char	*extr_red_flnm_frm_cm(t_cmd *cmd, char *start, char *end,
			t_redir *red)
{
	char	*quotes[MAX_QUOTES];
	char	*filename;
	char	*filename_aux;
	char	*aux;

	ft_bzero(quotes, sizeof(quotes));
	get_quotes_in_cm_sbstr(cmd, start, end, quotes);
	if (!quotes[0])
		return (ft_substr(start, 0, end - start + 1));
	if (red->kind == HEREDOC)
		red->expand_heredoc_content = 1;
	filename = extract_redir_filename_while_quoted(quotes, &start);
	if (start <= end)
	{
		aux = ft_substr(start, 0, end - start + 1);
		filename_aux = ft_strjoin(filename, aux);
		return (free(aux), free(filename), filename_aux);
	}
	return (filename);
}
