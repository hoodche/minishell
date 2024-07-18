/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 11:37:37 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/17 12:59:15 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief	once a $ sign was found (pointed by let), followed by "_" or an 
 * 			alphanumeric char (we are expanding a varenv then) this function
 * 			gets the length of the environment variable name. It also moves
 * 			let pointer to the end of the variable name
 * @param	char **let pointer to char in command string where the $ sign is
 */
size_t	get_envvar_name_length(char **let)
{
	size_t	var_name_length;

	var_name_length = 1;
	*let += 2;
	while (**let && (ft_isalnum(**let) || **let == '_'))
	{
		(*let)++;
		var_name_length++;
	}
	return (var_name_length);
}

/**
 * @brief	expands $? to exit status
 * @param	t_cmd *cmd pointer to command where the expansion is done
 * @param	char **let pointer to char in command string where the $ sign is
 * @param	char * old_orig poitr to origin of cmd str prior to expansion 
 * @param	size_t dol_offset. offset in bytes where the $ sign is
 */
void	exp_dol_qestn_mark(t_cmd *cmd, char **let, char *old_or, size_t dol_of)
{
	char	*aux;
	int		exp_size;

	aux = ft_itoa(g_status);
	exp_size = expand(cmd, *let, 2, aux);
	free(aux);
	*let = cmd->str + dol_of + exp_size + 1;
	repoint_ptrs_in_cm(cmd, old_or, dol_of, exp_size);
}

/**
 * @brief	expands $"..." or $'...' to nothing (deletes $ sign)
 * @param	t_cmd *cmd pointer to command where the expansion is done
 * @param	char **let pointer to char in command string where the $ sign is
 * @param	char * old_orig poitr to origin of cmd str prior to expansion 
 * @param	size_t dol_offset. offset in bytes where the $ sign is
 */
void	exp_dol_to_empty(t_cmd *cmd, char **let, char *old_or, size_t dol_of)
{
	int		exp_size;

	exp_size = expand(cmd, *let, 1, "");
	*let = cmd->str + dol_of + exp_size + 1;
	repoint_ptrs_in_cm(cmd, old_or, dol_of, exp_size);
}

/**
 * @brief	once a $ sign was found (pointed by let), this function decides what
 * 			to do with it (how to expand it) depending on the char following it.
 * 			-	Expands to exit status in ln 90.
 * 			-	Expands to nothing in ln 93.-deletes $ sign-
 * 			-	Expands to environment variable in ln 95. (line 105 pointer is
 * 												 moved right after expansion)
 * 			-	Otherwise, $ remains as a literal char (line 107)
 * @param	t_read_input	*input struct with cmds array to be parsed for $
 * 			expansions
 * @param	int index of command being parsed in cmds array (input->cmds[i])
 * @param	char **let pointer to char in command string where the $ sign is
 */
void	switch_case_dollar(t_read_input *input, int i, char **let)
{
	char	*origin;
	char	*aux;
	size_t	dol_offset;
	size_t	var_name_length;
	int		exp_size;

	origin = input->cmds[i].str;
	dol_offset = *let - origin;
	if (*(*let + 1) == '?')
		exp_dol_qestn_mark(&input->cmds[i], let, origin, dol_offset);
	else if (*(*let + 1) == '\'' || *(*let + 1) == '\"')
		exp_dol_to_empty(&input->cmds[i], let, origin, dol_offset);
	else if (*(*let + 1) == '_' || ft_isalpha(*(*let + 1)))
	{
		var_name_length = get_envvar_name_length(let);
		aux = ft_substr(origin, dol_offset + 1, var_name_length);
		exp_size = expand(&input->cmds[i], origin + dol_offset,
				1 + var_name_length, mini_getenv_value(aux, input->env));
		free(aux);
		*let = input->cmds[i].str + dol_offset + exp_size + 1;
		repoint_ptrs_in_cm(&input->cmds[i], origin, dol_offset, exp_size);
	}
	else
		(*let)++;
}
