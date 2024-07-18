/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 14:28:34 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/18 21:03:16 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//lines 72, 76, 78 and 83 ft_standard_error("", data); exit prog
static int	heredoc_write_temp(t_read_input *in, t_redir *r, int fd)
{
	char	*line;

	line = readline("> ");
	if (line == NULL)
		return (g_status = 0, ERROR);
	while (ft_strncmp(line, r->delimeter, ft_strlen(r->delimeter) + 1) != 0)
	{
		if (r->expand_heredoc_content == 0
			&& write_and_expand_heredoc(in, fd, line) == ERROR)
			return (free(line), ERROR);
		else if (r->expand_heredoc_content == 1
			&& (write(fd, line, ft_strlen(line)) < 0 || write(fd, "\n", 1) < 0))
			return (free(line), ERROR);
		free(line);
		if (g_status == 130)
			return (ERROR);
		line = readline("> ");
		if (line == NULL)
			return (g_status = 0, ERROR);
	}
	return (g_status = 0, free(line), OK);
}

// line 105 exit prog
static int	heredoc_readin(t_read_input *in, t_redir *r)
{
	int		temp_fd;
	char	*temp;

	temp = ft_itoa(in->heredoc_count);
	if (temp == NULL)
		mini_program_error(in);
	r->filename = ft_strjoin(F_TEMP, temp);
	if (r->filename == NULL)
		return (free(temp), mini_program_error(in), ERROR);
	free(temp);
	temp_fd = open(r->filename, O_WRONLY | O_CREAT, 0666);
	if (temp_fd == -1)
		mini_program_error(in);
	if (heredoc_write_temp(in, r, temp_fd) == ERROR)
	{
		close(temp_fd);
		return (ERROR);
	}
	close(temp_fd);
	return (OK);
}

int	mini_heredoc_process(t_read_input *in)
{
	int	i;
	int	j;

	i = 0;
	while (i < in->cmd_count)
	{
		j = 0;
		while (in->cmds[i].redir[j].kind != 0)
		{
			if (in->cmds[i].redir[j].kind == HEREDOC)
			{
				if (heredoc_readin(in, &in->cmds[i].redir[j]) == ERROR)
					return (ERROR);
				in->heredoc_count++;
			}
			j++;
		}
		i++;
	}
	return (OK);
}

void	mini_heredoc_del_temp(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->redir[i].kind != 0)
	{
		if (cmd->redir[i].kind == HEREDOC && cmd->redir[i].filename)
			unlink(cmd->redir[i].filename);
		i++;
	}
}
