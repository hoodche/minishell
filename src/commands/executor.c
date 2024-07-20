/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 19:40:15 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/20 16:44:46 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//t_read_input *in FOR ERR
// line 42exit prog
static int	is_executable_wrong_format(t_cmd *cmd)
{
	DIR				*d;
	struct dirent	*dir;
	size_t			len;

	d = opendir(".");
	if (!d)
		perror("");
	dir = readdir(d);
	while (dir)
	{
		len = ft_strlen(cmd->argv_for_execve[0]);
		if (ft_strncmp(dir->d_name, cmd->argv_for_execve[0], len) == 0
			&& dir->d_type == DT_REG)
			return (closedir(d), true);
		dir = readdir(d);
	}
	closedir(d);
	return (false);
}

//line 73 error mem, free_matrix(args, in);
static int	find_cmd_path(t_read_input *in, t_cmd *cmd)
{
	char	*cmd_path;
	int		i;

	if (ft_startswith(cmd->argv_for_execve[0], "/") == true
		|| ft_startswith(cmd->argv_for_execve[0], "./") == true)
		return (OK);
	i = 0;
	if (in->cmd_paths == NULL || cmd->argv_for_execve[0] == NULL)
		return (ERROR);
	while (in->cmd_paths[i])
	{
		cmd_path = ft_join_strs(in->cmd_paths[i], "/", cmd->argv_for_execve[0]);
		if (cmd_path == NULL)
			return (ERROR);
		if (access(cmd_path, X_OK) == 0)
		{
			free(cmd->argv_for_execve[0]);
			cmd->argv_for_execve[0] = cmd_path;
			return (OK);
		}
		free(cmd_path);
		i++;
	}
	return (ERROR);
}

static int	execve_cmd_process(t_read_input *in, t_cmd *cmd)
{
	char	**envs;
	int		is_exe_wrong;

	is_exe_wrong = false;
	if (cmd->argv_for_execve[0] == NULL)
		return (g_status = 0, true);
	if (find_cmd_path(in, cmd) == ERROR)
		is_exe_wrong = is_executable_wrong_format(cmd);
	envs = makes_env_matrix_from_env_list(in->env);
	if (is_exe_wrong == false)
		execve(cmd->argv_for_execve[0], cmd->argv_for_execve, envs);
	free_matrix(envs);
	return (false);
}

static void	restore_fds(int copied_fd, int to_restore_fd)
{
	if (copied_fd > ERROR)
	{
		dup2(copied_fd, to_restore_fd);
		close(copied_fd);
	}
}

void	ft_cmd_exec(t_read_input *in, t_cmd *cmd, int write_pipe)
{
	int		copy[2];
	int		restore[2];
	int		is_builtin;
	int		exit;

	restore[0] = inredir_process(in, cmd, write_pipe - 1, &copy[0]);
	if (restore[0] == ERROR)
		return ;
	restore[1] = outred_process(in, cmd, write_pipe, &copy[1]);
	if (restore[1] == ERROR)
		return ;
	mini_close_fds(in, cmd);
	is_builtin = cmd_is_builtin(cmd);
	if (is_builtin == NOT_BUILTIN)
		exit = execve_cmd_process(in, cmd);
	else
		exit = mini_builtin_process(in, cmd, is_builtin);
	restore_fds(copy[0], restore[0]);
	restore_fds(copy[1], restore[1]);
	if (exit)
		mini_destroy_and_exit(in);
	else if (exit  == false && is_builtin == NOT_BUILTIN)
		error_execve(in, cmd);
}
