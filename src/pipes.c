/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 19:41:33 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/22 17:20:20 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_pipes_init(t_read_input *in)
{
	int	i;

	in->pipe_fd = malloc(in->pipe_count * sizeof(int *));
	if (in->pipe_fd == NULL)
		return ;
	i = 0;
	while (i < in->pipe_count)
	{
		in->pipe_fd[i] = malloc(2 * sizeof(int));
		if (in->pipe_fd[i] == NULL)
			return ;
		ft_bzero(in->pipe_fd[i], 2 * sizeof(int));
		i++;
	}
}

void	ft_pipes_start(t_read_input *in)
{
	int	i;

	i = 0;
	while (i < in->pipe_count)
	{
		if (pipe(in->pipe_fd[i]) < 0)
			perror("failed pipe");
		i++;
	}
}

void	ft_pipes_close(t_read_input *in)
{
	int	i;

	i = 0;
	while (i < in->pipe_count)
	{
		close(in->pipe_fd[i][READ_END]);
		close(in->pipe_fd[i][WRITE_END]);
		i++;
	}
}

void	ft_pipes_destroy(t_read_input *in)
{
	int	i;

	if (in->pipe_count == 0)
		return ;
	i = 0;
	while (i < in->pipe_count)
	{
		free(in->pipe_fd[i]);
		i++;
	}
	free(in->pipe_fd);
}
