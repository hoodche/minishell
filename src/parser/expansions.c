/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:46:06 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 21:59:16 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief	reallocs str to new_size bytes
 * @param	char *str to be reallocated 
 * @param	size_t new_size size in bytes of new str
 * @return	char *ptr to reallocated string
 */
char	*ft_realloc(char *str, size_t new_size)
{
	char	*new_str;

	new_str = malloc(new_size);
	if (!new_str)
		return (NULL);
	ft_bzero(new_str, new_size);
	if (ft_strlen(str) < new_size)
		ft_memmove(new_str, str, ft_strlen(str));
	else
		ft_memmove(new_str, str, new_size -1);
	return (new_str);
}

/**
 * @brief	expands something (variable, tilde,...) in a command string
 * 				-line 58 displaces remainder to the right making room for new
 * 				-line 60 inserts new in the gap
 * @param	t_cmd *cmd pointer to command where the expansion is to be done
 * @param	char * old poitr to char in command string where the expansion is to
 * 			be done
 * @param	size_t size in bytes of substring to be replaced
 * @param	char *new pter to string to be inserted in place of old (must be 
 * 			null-terminated)
 * @return	int difference in bytes between old and new strings
 */
int	expand(t_cmd *cmd, char *old, size_t size, char *new)
{
	int		bytes_diff;
	int		old_offset;
	char	*aux;

	if (!new)
		new = "";
	bytes_diff = ft_strlen(new) - size;
	old_offset = old - cmd->str;
	aux = ft_realloc(cmd->str, ft_strlen(cmd->str) + bytes_diff + 1);
	ft_memmove(aux + old_offset + ft_strlen(new), cmd->str + old_offset + size,
		ft_strlen(cmd->str + old_offset + size));
	ft_memmove(aux + old_offset, new, ft_strlen(new));
	free(cmd->str);
	cmd->str = aux;
	return (bytes_diff);
}

/**
 * @brief	restores quotes pointers to ( the new) cmd string after an expansion
 * 				- if quote sign is after expansion, recalculate it (lines 82 and
 * 					 93)
 * 				- if quote sign is before expansion, keep it at same offset 
 * 					(lines 88 and 99 )
 * @param	t_cmd *cmd pointer to command where the expansion has been done
 * @param	char * old_origin poitr to origin of cmd str prior to expansion 
 * @param	size_t expansion_offset. offset in bytes where the expansion took
 * 			place (counted from old_origin)
 * @param	int *expansion_size bytes added or substracted to cmd str by 
 * 			expansion
 */
void	repoint_qt(t_cmd *cmd, char *old_orig, size_t exp_offset, int exp_size)
{
	int	i;

	i = 0;
	while (cmd->ruling_dbl_quotes[i])
	{
		if (cmd->ruling_dbl_quotes[i] > old_orig + exp_offset)
			cmd->ruling_dbl_quotes[i] = cmd->str + (cmd->ruling_dbl_quotes[i]
					- old_orig) + exp_size;
		else
			cmd->ruling_dbl_quotes[i] = cmd->str + (cmd->ruling_dbl_quotes[i]
					- old_orig);
		i++;
	}
	i = 0;
	while (cmd->ruling_sgl_quotes[i])
	{
		if (cmd->ruling_sgl_quotes[i] > old_orig + exp_offset)
			cmd->ruling_sgl_quotes[i] = cmd->str + (cmd->ruling_sgl_quotes[i]
					- old_orig) + exp_size;
		else
			cmd->ruling_sgl_quotes[i] = cmd->str + (cmd->ruling_sgl_quotes[i]
					- old_orig);
		i++;
	}
}

/**
 * @brief	restores pointers to ( the new) cmd string after an expansion
 * 				- if redir sign is after expansion, recalculate it (line 120)
 * 				- if redir sign is before expansion, keep it at same offset 
 * 					(ln 124)
 * @param	t_cmd *cmd pointer to command where the expansion has been done
 * @param	char * old_orig poitr to origin of cmd str prior to expansion 
 * @param	size_t exp_offset. offset in bytes where the expansion took
 * 			place (counted from old_orig)
 * @param	int *exp_size bytes added or substracted to cmd str by 
 * 			expansion
 */
void	repoint_ptrs_in_cm(t_cmd *cmd, char *old_or, size_t exp_of, int exp_sz)
{
	int	i;

	repoint_qt(cmd, old_or, exp_of, exp_sz);
	i = 0;
	while (cmd->redirec_ptr[i])
	{
		if (cmd->redirec_ptr[i] > old_or + exp_of)
			cmd->redirec_ptr[i] = cmd->str + (cmd->redirec_ptr[i]
					- old_or) + exp_sz;
		else
			cmd->redirec_ptr[i] = cmd->str + (cmd->redirec_ptr[i] - old_or);
		i++;
	}
}

/**
 * @brief	parses all commands looking for '$' to be expanded
 * @param	t_read_input	*input struct with cmds array to be parsed for $
 * 			expansions
 */
void	expand_dollar(t_read_input *input)
{
	int		i;
	char	*let;

	i = -1;
	while (++i < input->cmd_count)
	{
		let = input->cmds[i].str;
		while (*let)
		{
			if (!is_sgl_quoted_in_cmd(&input->cmds[i], let) && (*let == '$')
				&& !is_in_heredoc_delimeter(&input->cmds[i], let))
				switch_case_dollar(input, i, &let);
			else
				let++;
		}
	}
}
