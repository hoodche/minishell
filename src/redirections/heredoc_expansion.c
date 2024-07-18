/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:09:23 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/18 19:27:45 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_env_identifier(char *s, size_t *i)
{
	size_t	len;

	*i = 1;
	if (s && ft_isdigit(*s))
		return (ft_substr(s, 0, 1));
	len = ft_strlen(s);
	*i = 0;
	while (*i < len && (ft_isalnum(s[*i]) == true || s[*i] == '_'))
		(*i)++;
	if (*i == 0)
		return (NULL);
	return (ft_substr(s, 0, *i));
}

static int	write_env_heredoc(t_read_input *in, char *loop, int fd)
{
	char	*key;
	char	*value;
	size_t	i;

	key = get_env_identifier(loop + 1, &i);
	if (key)
	{
		value = mini_getenv_value(key, in->env);
		if (value && write(fd, value, ft_strlen(value)) < 0)
			return (free(key), ERROR);
		free(key);
		return (i + 1);
	}
	return (false);
}

int	write_and_expand_heredoc(t_read_input *in, int fd, char *line)
{
	char	*loop;
	int		ret;

	loop = ft_strchr(line, '$');
	while (loop != NULL)
	{
		if ((loop - line) > 0 && write(fd, line, loop - line) < 0)
			return (ERROR);
		ret = write_env_heredoc(in, loop, fd);
		if (ret == ERROR)
			return (ERROR);
		else if (ret)
		{
			line += (loop - line) + ret;
			loop = ft_strchr(line, '$');
		}
		else
		{
			loop = NULL;
			line++;
		}
	}
	if (write(fd, line, ft_strlen(line)) < 0 || write(fd, "\n", 1) < 0)
		return (ERROR);
	return (OK);
}
