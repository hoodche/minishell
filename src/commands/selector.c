/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selector.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 19:05:33 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/18 21:19:05 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	single_execution(t_read_input *in)
{
	int	is_builtin;

	if (in->cmds[0].argv_for_execve[0] == NULL)
	{
		mini_heredoc_del_temp(&in->cmds[0]);
		g_status = 0;
		return ;
	}
	is_builtin = cmd_is_builtin(&in->cmds[0]);
	if (is_builtin == IS_EXPORT || is_builtin == IS_UNSET
		|| is_builtin == IS_EXIT || is_builtin == IS_CD)
		ft_cmd_exec(in, &in->cmds[0], 0);
	else
	{
		ft_fork_only_child(in);
		ft_wait_children(in);
	}
}

int	mini_select_execution(t_read_input *in)
{
	update_cmd_paths(in);
	if (mini_heredoc_process(in) == ERROR)
	{
		mini_heredoc_del_temp(&in->cmds[0]);
		return (ERROR);
	}
	if (in->cmd_count == 1)
		single_execution(in);
	else
	{
		in->pipe_count = in->cmd_count - 1;
		ft_pipes_init(in);
		ft_pipes_start(in);
		ft_fork_children(in);
		ft_pipes_close(in);
		ft_wait_children(in);
	}
	return (OK);
}
