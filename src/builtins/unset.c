/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:41:55 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/18 18:34:47 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	unset_pwd(char *key, t_dlist **env)
{
	t_dlist	*to_delete;

	to_delete = mini_getenv(key, *env);
	if (to_delete == NULL)
		return ;
	if (to_delete->value)
		free(to_delete->value);
	to_delete->value = NULL;
	to_delete->status = CHANGED_UNSET;
}

void	mini_unset(t_read_input *in, t_cmd *cmd)
{
	int	i;

	if (cmd_count_args(cmd) > 1)
	{
		if (invalid_flags_standard(cmd) == true)
			return ;
		i = 1;
		while (cmd->argv_for_execve[i])
		{
			if (!ft_strncmp(cmd->argv_for_execve[i], "PWD", 3)
				&& ft_strlen(cmd->argv_for_execve[i]) == 3)
				unset_pwd(cmd->argv_for_execve[i], &in->env);
			else
				mini_unsetenv(cmd->argv_for_execve[i], &in->env);
			i++;
		}
	}
	g_status = 0;
}
