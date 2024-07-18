/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 19:35:37 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/18 12:59:18 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	inredirs_exists(t_cmd *cmd)
{
	int	i;
	int	count_in;

	i = 0;
	count_in = 0;
	while (cmd->redir[i].kind)
	{
		if (cmd->redir[i].kind == IN || cmd->redir[i].kind == HEREDOC)
			count_in++;
		i++;
	}
	return (count_in);
}

static int	find_last_inredir(t_cmd *cmd)
{
	int		i;
	int		last_in;

	i = 0;
	last_in = 0;
	while (cmd->redir[i].kind)
	{
		if (cmd->redir[i].kind == IN || cmd->redir[i].kind == HEREDOC)
		{
			if (access(cmd->redir[i].filename, F_OK) == -1
				|| access(cmd->redir[i].filename, R_OK) == -1)
				return (mini_errno_error(1, cmd->redir[i].filename, NULL),
					ERROR);
			last_in = i;
		}
		i++;
	}
	return (last_in);
}

//line 73 ft_file_error(data->in, data);
// line 76 check if valid fd in redir???
int	inredir_process(t_read_input *in, t_cmd *cmd, int pread, int *copy_in)
{
	int		last_in;

	*copy_in = -1;
	if (inredirs_exists(cmd) == false && pread >= 0 && in->cmd_count > 1)
	{
		if (dup2(in->pipe_fd[pread][READ_END], STDIN_FILENO) < 0)
			perror("");
		return (STDIN_FILENO);
	}
	else if (inredirs_exists(cmd) == false)
		return (false);
	last_in = find_last_inredir(cmd);
	if (last_in == ERROR)
		return (ERROR);
	cmd->fd_infile = open(cmd->redir[last_in].filename, O_RDONLY);
	if (cmd->fd_infile == -1)
		return (ERROR);
	if (in->cmd_count == 1)
		*copy_in = dup(cmd->redir[last_in].fd);
	dup2(cmd->fd_infile, cmd->redir[last_in].fd);
	return (cmd->redir[last_in].fd);
}
