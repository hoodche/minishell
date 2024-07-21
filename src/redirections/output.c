/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 19:35:37 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/21 14:51:25 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	outredirs_exists(t_cmd *cmd)
{
	int	i;
	int	count_out;

	i = 0;
	count_out = 0;
	while (cmd->redir[i].kind)
	{
		if (cmd->redir[i].kind == OUT || cmd->redir[i].kind == APPEND)
			count_out++;
		i++;
	}
	if (count_out == 0)
		return (false);
	return (true);
}

static int	outredir_open(t_redir r)
{
	int	fd;

	if (r.kind == APPEND)
		fd = open(r.filename,
				O_APPEND | O_WRONLY | O_CREAT, 0666);
	else
		fd = open(r.filename, O_WRONLY | O_CREAT, 0666);
	return (fd);
}

static int	find_last_outredir(t_cmd *cmd)
{
	int	i;
	int	last_out;
	int	fd;

	i = 0;
	last_out = 0;
	while (cmd->redir[i].kind)
	{
		if (cmd->redir[i].kind == OUT || cmd->redir[i].kind == APPEND)
		{
			if (access(cmd->redir[i].filename, F_OK) == 0
				&& access(cmd->redir[i].filename, W_OK) == -1)
				return (mini_errno_error(1, cmd->redir[i].filename, NULL),
					ERROR);
			fd = outredir_open(cmd->redir[i]);
			close(fd);
			last_out = i;
		}
		i++;
	}
	return (last_out);
}

void	find_first_outredir(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (cmd->redir[i].kind)
	{
		if (cmd->redir[i].kind == OUT || cmd->redir[i].kind == APPEND)
		{
			if (access(cmd->redir[i].filename, F_OK) == 0
				&& access(cmd->redir[i].filename, W_OK) == -1)
				return (mini_errno_error(1, cmd->redir[i].filename, NULL));
			fd = outredir_open(cmd->redir[i]);
			close(fd);
			return ;
		}
		i++;
	}
}
//line 90 ft_file_error(cmd->redir[*last_out].filename, data);EXIT PROG
// line 93 check if valid .fd in redir???
int	outred_process(t_read_input *in, t_cmd *cmd, int pwrite, int *copy_out)
{
	int		last_out;

	*copy_out = -1;
	if (outredirs_exists(cmd) == false && pwrite < in->pipe_count)
	{
		if (dup2(in->pipe_fd[pwrite][WRITE_END], STDOUT_FILENO) < 0)
			perror("");
		return (STDOUT_FILENO);
	}
	else if (outredirs_exists(cmd) == false)
		return (false);
	last_out = find_last_outredir(cmd);
	if (last_out == ERROR)
		return (ERROR);
	if (cmd->redir[last_out].kind == OUT
		&& access(cmd->redir[last_out].filename, F_OK) == 0)
		unlink(cmd->redir[last_out].filename);
	cmd->fd_outredir = outredir_open(cmd->redir[last_out]);
	if (cmd->fd_outredir == -1)
		return (ERROR);
	if (in->cmd_count == 1)
		*copy_out = dup(cmd->redir[last_out].fd);
	dup2(cmd->fd_outredir, cmd->redir[last_out].fd);
	return (cmd->redir[last_out].fd);
}
