/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 19:40:04 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/13 21:35:02 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief	frees resources inside a cmd in input->cmds array
 * @param	t_cmd *cmd struct to be parsed for freeing resources
 */
void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (cmd->str)
		free(cmd->str);
	i = -1;
	while (cmd->redir[++i].kind)
	{
		if (cmd->redir[i].filename)
			free(cmd->redir[i].filename);
		if (cmd->redir[i].delimeter)
			free(cmd->redir[i].delimeter);
	}
	i = -1;
	while (cmd->argv_for_execve[++i])
		free(cmd->argv_for_execve[i]);
}

/**
 * @brief	frees all resources (env list and struct) before minishell exits 
 * 			completely (no new prompt issued)
 * @param	t_read_input	*input struct to be parsed for freeing resources
 */
void	mini_destroy_and_exit(t_read_input *in)
{
	ft_dlistclear(&in->env);
	free_matrix(in->cmd_paths);
	free_resources(in);
	exit(g_status);
}

/**
 * @brief	frees resources in input struct before issuing a new prompt
 * @param	t_read_input	*input struct to be parsed for freeing resources
 */
void	free_resources(t_read_input *in)
{
	int	i;

	if (in->typed)
		free(in->typed);
	i = -1;
	while (++i < in->cmd_count)
		free_cmd(&in->cmds[i]);
	ft_pipes_destroy(in);
}
