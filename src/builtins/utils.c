/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 19:40:43 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/18 21:37:47 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_is_builtin(t_cmd *cmd)
{
	size_t	len_cmd;

	len_cmd = ft_strlen(cmd->argv_for_execve[0]);
	if (!ft_strncmp(cmd->argv_for_execve[0], S_ECHO, ft_strlen(S_ECHO))
		&& len_cmd == ft_strlen(S_ECHO))
		return (IS_ECHO);
	else if (!ft_strncmp(cmd->argv_for_execve[0], S_CD, ft_strlen(S_CD))
		&& len_cmd == ft_strlen(S_CD))
		return (IS_CD);
	else if (!ft_strncmp(cmd->argv_for_execve[0], S_PWD, ft_strlen(S_PWD))
		&& len_cmd == ft_strlen(S_PWD))
		return (IS_PWD);
	else if (!ft_strncmp(cmd->argv_for_execve[0], S_EXPORT, ft_strlen(S_EXPORT))
		&& len_cmd == ft_strlen(S_EXPORT))
		return (IS_EXPORT);
	else if (!ft_strncmp(cmd->argv_for_execve[0], S_UNSET, ft_strlen(S_UNSET))
		&& len_cmd == ft_strlen(S_UNSET))
		return (IS_UNSET);
	else if (!ft_strncmp(cmd->argv_for_execve[0], S_ENV, ft_strlen(S_ENV))
		&& len_cmd == ft_strlen(S_ENV))
		return (IS_ENV);
	else if (!ft_strncmp(cmd->argv_for_execve[0], S_EXIT, ft_strlen(S_EXIT))
		&& len_cmd == ft_strlen(S_EXIT))
		return (IS_EXIT);
	return (NOT_BUILTIN);
}

int	invalid_flags_standard(t_cmd *cmd)
{
	size_t	len;
	char	*arg;

	arg = cmd->argv_for_execve[1];
	len = ft_strlen(arg);
	if ((len == 2 && arg[0] == '-' && ft_strncmp(arg, "--", 2) != 0)
		|| (len > 2 && arg[0] == '-'))
	{
		printf("%s: -%c: invalid option\n", cmd->argv_for_execve[0], arg[1]);
		g_status = 2;
		return (true);
	}
	return (false);
}

int	cmd_count_args(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd->argv_for_execve && cmd->argv_for_execve[count])
		count++;
	return (count);
}

void	mini_unsetenv(char *key, t_dlist **env)
{
	t_dlist	*to_delete;
	t_dlist	*prev;
	t_dlist	*next;

	to_delete = mini_getenv(key, *env);
	if (to_delete == NULL)
		return ;
	prev = to_delete->prev;
	next = to_delete->next;
	if (prev == NULL)
		*env = next;
	else
		prev->next = next;
	if (next)
		next->prev = prev;
	if (to_delete->backup)
		free(to_delete->backup);
	if (to_delete->key)
		free(to_delete->key);
	if (to_delete->value)
		free(to_delete->value);
	free(to_delete);
}

int	mini_builtin_process(t_read_input *in, t_cmd *cmd, int type)
{
	if (type == IS_ECHO)
		mini_echo(in, cmd);
	else if (type == IS_CD)
		mini_cd(in, cmd);
	else if (type == IS_PWD)
		mini_pwd(in, cmd);
	else if (type == IS_EXPORT)
		mini_export(in, cmd);
	else if (type == IS_UNSET)
		mini_unset(in, cmd);
	else if (type == IS_ENV)
		mini_env(in, cmd);
	else if (type == IS_EXIT)
		mini_exit(in, cmd);
	if (in->cmd_count == 1 && (type == IS_EXPORT || type == IS_UNSET
			|| type == IS_CD))
		return (false);
	return (true);
}
