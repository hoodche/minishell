/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:41:20 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/13 22:22:00 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	mini_env(t_read_input *in, t_cmd *cmd)
{
	t_dlist	*env;

	if (cmd_count_args(cmd) > 1)
		return (mini_formatted_error(1, S_ENV, NULL, "too many arguments"));
	env = in->env;
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	g_status = 0;
}
