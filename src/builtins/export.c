/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:41:30 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/19 20:34:17 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	export_is_valid_identifier(char *s)
{
	char	*eq_symbol;

	if (s && ft_isdigit(*s))
		return (g_status = 1, false);
	eq_symbol = ft_strchr(s, '=');
	if (eq_symbol == NULL)
		eq_symbol = s + ft_strlen(s);
	if (s == eq_symbol)
		return (mini_formatted_error(1, S_EXPORT, s, ERR_EXPORT_ID), false);
	while (s && *s && s < eq_symbol)
	{
		if (ft_isalnum(*s) == false && *s != '_')
			return (mini_formatted_error(1, S_EXPORT, s, ERR_EXPORT_ID), false);
		s++;
	}
	return (true);
}

static void	mini_export_setter(t_read_input *in, char *var)
{
	char	*eq_symbol;
	char	*key;

	eq_symbol = ft_strchr(var, '=');
	if (eq_symbol == NULL)
		return ;
	key = ft_substr(var, 0, eq_symbol - var);
	mini_setenv(key, eq_symbol + 1, CHANGED_EXPORT, &in->env);
	free(key);
}

static void	export_print_dlist(t_read_input *in)
{
	t_dlist	*env;

	env = in->env;
	while (env)
	{
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		env = env->next;
	}
	g_status = 0;
}

void	mini_export(t_read_input *in, t_cmd *cmd)
{
	int		i;
	char	*current_arg;

	if (cmd_count_args(cmd) == 1)
		return (export_print_dlist(in));
	else if (invalid_flags_standard(cmd) == true)
		return ;
	i = 1;
	g_status = 0;
	if (ft_strncmp(cmd->argv_for_execve[i], "--", 2) == 0)
		i++;
	while (cmd->argv_for_execve[i])
	{
		current_arg = cmd->argv_for_execve[i];
		if (export_is_valid_identifier(current_arg) == true)
			mini_export_setter(in, current_arg);
		i++;
	}
}
