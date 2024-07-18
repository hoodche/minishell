/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:41:47 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/14 15:55:34 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	mini_pwd(t_read_input *in, t_cmd *cmd)
{
	t_dlist	*l;

	if (cmd_count_args(cmd) > 1 && invalid_flags_standard(cmd) == true)
		return ;
	l = mini_getenv("PWD", in->env);
	if (l->status <= CHANGED_CD)
		printf("%s\n", l->value);
	else if (l->status > CHANGED_CD)
		printf("%s\n", l->backup);
	g_status = 0;
}
