/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 19:03:56 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/22 17:20:39 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	mini_close_fds(t_read_input *in, t_cmd *cmd)
{
	ft_pipes_close(in);
	if (cmd->fd_infile > 2)
		close(cmd->fd_infile);
	if (cmd->fd_outredir > 2)
		close(cmd->fd_outredir);
	mini_heredoc_del_temp(cmd);
}

/**
 * @brief	frees all strings inside a str matrix (but not the matrix pointer
 * 			itself)
 * @param	char** matrix
 */
void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	if (matrix == NULL)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
	matrix = NULL;
}
