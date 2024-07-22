/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:41:12 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/22 17:16:32 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	echo_flags(t_read_input *in, t_cmd *cmd)
{
	size_t	i;
	size_t	len;
	char	*trimmed;

	i = 1;
	while (cmd->argv_for_execve[i]
		&& ft_strncmp(cmd->argv_for_execve[i], "-n", 2) == 0)
	{
		trimmed = ft_strtrim(cmd->argv_for_execve[i] + 1, "n");
		if (trimmed == NULL)
			mini_program_error(in);
		len = ft_strlen(trimmed);
		free(trimmed);
		if (len != 0)
			return (i);
		i++;
	}
	return (i);
}

void	mini_echo(t_read_input *in, t_cmd *cmd)
{
	size_t	i;
	int		flags_count;

	i = echo_flags(in, cmd);
	flags_count = i - 1;
	while (cmd->argv_for_execve[i])
	{
		if (cmd->argv_for_execve[i + 1])
			printf("%s ", cmd->argv_for_execve[i]);
		else
			printf("%s", cmd->argv_for_execve[i]);
		i++;
	}
	if (flags_count == 0)
		printf("\n");
	g_status = 0;
}
