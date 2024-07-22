/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 21:18:46 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/22 17:17:15 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	update_cmd_paths(t_read_input *in)
{
	char	*value;

	if (in->cmd_paths)
	{
		free_matrix(in->cmd_paths);
		in->cmd_paths = NULL;
	}
	value = mini_getenv_value(ENV_PATH, in->env);
	if (value == NULL)
		return ;
	in->cmd_paths = ft_split(value, ':');
	if (in->cmd_paths == NULL)
		return (mini_program_error(in));
}
