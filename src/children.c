/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 19:41:12 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/18 21:06:51 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//line 20 ft_standard_error("", data);
//line 23 ft_process_children(data, cmds[i], i);
void	ft_fork_only_child(t_read_input *in)
{
	in->cmds[0].pid = fork();
	if (in->cmds[0].pid == -1)
		return ;
	else if (in->cmds[0].pid == 0)
	{
		ft_cmd_exec(in, &in->cmds[0], 0);
		mini_destroy_and_exit(in);
	}
}

//line 41 ft_standard_error("", data);
void	ft_fork_children(t_read_input *in)
{
	int	i;

	i = 0;
	while (i < in->cmd_count)
	{
		in->cmds[i].pid = fork();
		if (in->cmds[i].pid == -1)
		{
			ft_pipes_close(in);
			perror("");
			return ;
		}
		else if (in->cmds[i].pid == 0)
		{
			ft_cmd_exec(in, &in->cmds[i], i);
			mini_destroy_and_exit(in);
		}
		i++;
	}
}

// line 60 so that SIgquit works if a child process is running and blocked
// line 62 so that parent process do not print a new promt
// line 67 ft_standard_error("", data);
// line 75 child normal termination
// line 77 This macro returns a nonzero value if the child process terminated 
//		because it received a signal that was not handled.
// line 79  because the child process terminated without printing a new line
void	ft_wait_children(t_read_input *in)
{
	int	i;
	int	status;

	i = 0;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_IGN);
	while (i < in->cmd_count)
	{
		if (waitpid(in->cmds[i].pid, &status, WUNTRACED) < 0)
			return ;
		i++;
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
	if (WIFEXITED(status))
		g_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		printf("\n");
		g_status = WTERMSIG(status) + 128;
	}
}
