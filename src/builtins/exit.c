/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:34:00 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/18 12:41:54 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	exit_valid_numeric_arg(t_cmd *cmd)
{
	int			i;

	i = 0;
	while (cmd->argv_for_execve[1][i])
	{
		if (ft_isdigit(cmd->argv_for_execve[1][i]) == false)
			return (mini_formatted_error(2, S_EXIT,
					cmd->argv_for_execve[1], ERR_EXIT_ARG), false);
		i++;
	}
	return (true);
}

void	mini_exit(t_read_input *in, t_cmd *cmd)
{
	int	count_args;
	int	exit_num;
	int	is_numeric;

	count_args = cmd_count_args(cmd);
	printf("exit\n");
	if (count_args >= 2)
		is_numeric = exit_valid_numeric_arg(cmd);
	if (count_args > 2 && is_numeric == true)
		return (mini_formatted_error(1, S_EXIT, NULL, "too many arguments"));
	else if (count_args == 2 && is_numeric == true)
		exit_num = ft_atoi(cmd->argv_for_execve[1]);
	else
		exit_num = g_status;
	if (exit_num > 255)
		exit_num %= 256;
	else if (exit_num < 0)
	{
		while (exit_num < 0)
			exit_num += 256;
	}
	g_status = exit_num;
	mini_destroy_and_exit(in);
}
