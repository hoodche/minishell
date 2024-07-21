/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 18:41:03 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/21 15:15:14 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//line 72, 76 and 85 error mem, exit prog
char	*cd_get_abspath(t_read_input *in, char *new_dir)
{
	char	*pwd;
	char	**dirs;
	int		i;

	dirs = ft_split(new_dir, '/');
	if (dirs == NULL)
		return (NULL);
	i = 0;
	pwd = get_current_pwd(in);
	if (pwd == NULL)
		return (NULL);
	while (dirs[i])
	{
		if (ft_strncmp(dirs[i], "..", 2) == 0)
			pwd = delete_last_path(pwd);
		else if (ft_strncmp(dirs[i], ".", 1) && ft_strncmp(dirs[i], "..", 2))
		{
			pwd = add_path_to_pwd(pwd, dirs[i]);
			if (pwd == NULL)
				return (free_matrix(dirs), NULL);
		}
		i++;
	}
	pwd = check_pwd_empty(pwd);
	return (free_matrix(dirs), pwd);
}

static bool	ft_get_dir_path(t_read_input *in, t_cmd *cmd, char **new_dir)
{
	char	*cpath;
	char	**pths;
	int		i;

	if (ft_startswith(cmd->argv_for_execve[1], "/") == false)
	{
		cpath = mini_getenv_value("CDPATH", in->env);
		if (cpath && ft_strlen(cpath) != 0)
		{
			pths = ft_split(cpath, ':');
			i = 0;
			while (pths && pths[i])
			{
				*new_dir = ft_join_strs(pths[i], "/", cmd->argv_for_execve[1]);
				if (*new_dir == NULL)
					return (free_matrix(pths), mini_program_error(in), false);
				if (access(*new_dir, X_OK) == 0)
					return (free_matrix(pths), true);
				free(*new_dir);
				i++;
			}
		}
	}
	*new_dir = ft_strdup(cmd->argv_for_execve[1]);
	return (false);
}

//lines 55 and 61 exit program
static void	cd_update_vars(t_read_input *in, char *new_dir)
{
	char	*oldpwd;
	char	*pwd;
	t_dlist	*l;

	if (ft_startswith(new_dir, "/") == true)
	{
		pwd = (char *)malloc(PATH_MAX);
		if (pwd == NULL)
			return (free(new_dir), mini_program_error(in));
		getcwd(pwd, PATH_MAX);
	}
	else
		pwd = cd_get_abspath(in, new_dir);
	if (pwd == NULL)
		return (free(new_dir), mini_program_error(in));
	l = mini_getenv("PWD", in->env);
	oldpwd = l->value;
	mini_setenv("OLDPWD", oldpwd, CHANGED_CD, &in->env);
	mini_replace_env(l, pwd, CHANGED_CD);
	if (l->backup)
		free(l->backup);
	l->backup = ft_strdup(pwd);
	free(pwd);
}

//lines 79 and 84 exit program
static int	cd_loop_chdir(t_read_input *in, t_cmd *cmd, char *new_dir)
{
	char	**dirs;
	int		i;

	if (new_dir == NULL)
		return (mini_program_error(in), ERROR);
	if (ft_startswith(new_dir, "/") == true)
		chdir("/");
	dirs = ft_split(new_dir, '/');
	if (dirs == NULL)
		return (free(new_dir), mini_program_error(in), ERROR);
	i = 0;
	while (dirs[i])
	{
		if (chdir(dirs[i]) < 0)
		{
			free_matrix(dirs);
			free(new_dir);
			return (mini_errno_error(1, S_CD, cmd->argv_for_execve[1]), ERROR);
		}
		i++;
	}
	free_matrix(dirs);
	cd_update_vars(in, new_dir);
	return (OK);
}

void	mini_cd(t_read_input *in, t_cmd *cmd)
{
	char	*new_dir;
	bool	used_cdpath;

	used_cdpath = false;
	g_status = 0;
	new_dir = NULL;
	if (cmd_count_args(cmd) > 2)
		return (mini_formatted_error(1, S_CD, NULL, "too many arguments"));
	else if (cmd_count_args(cmd) == 1)
	{
		new_dir = mini_getenv_value("HOME", in->env);
		if (new_dir)
			new_dir = ft_strdup(new_dir);
		if (new_dir == NULL)
			return (mini_formatted_error(1, S_CD, NULL, "HOME not set"));
	}
	else
	{
		if (invalid_flags_standard(cmd) == true)
			return ;
		used_cdpath = ft_get_dir_path(in, cmd, &new_dir);
	}
	if (cd_loop_chdir(in, cmd, new_dir) == ERROR)
		return ;
	cd_print(used_cdpath, &new_dir);
}
