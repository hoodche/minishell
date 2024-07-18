/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 19:03:56 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/14 16:13:28 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
//TO-DO: temporal for now
int	ft_startswith(char *s, char *start)
{
	size_t	j;
	size_t	i;

	j = ft_strlen(start);
	if (ft_strlen(s) < j)
		return (false);
	i = 0;
	while (i < j)
	{
		if (s[i] != start[i])
			return (false);
		i++;
	}
	return (true);
}

char	*ft_join_strs(char *s1, char *s2, char *s3)
{
	char	*joined;
	size_t	size;

	size = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	joined = malloc(size + 1);
	if (joined == NULL)
		return (NULL);
	ft_bzero(joined, size + 1);
	ft_strlcat(joined, s1, size + 1);
	ft_strlcat(joined, s2, size + 1);
	ft_strlcat(joined, s3, size + 1);
	return (joined);
}

void	ft_free_split(char **s)
{
	int	i;

	if (s == NULL)
		return ;
	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

void	mini_close_fds(t_read_input *in, t_cmd *cmd)
{
	ft_pipes_close(in);
	if (cmd->fd_infile > 2)
		close(cmd->fd_infile);
	if (cmd->fd_outredir > 2)
		close(cmd->fd_outredir);
	mini_heredoc_del_temp(cmd);
}
