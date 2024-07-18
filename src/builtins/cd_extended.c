/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_extended.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:31:54 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/18 12:53:48 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cd_print(int used_cdpath, char **s)
{
	if (used_cdpath)
		printf("%s\n", *s);
	if (s && *s)
		free(*s);
}

static char	*delete_last_path(char *pwd)
{
	char	*temp;

	temp = ft_strrchr(pwd, '/');
	if (temp != NULL)
	{
		temp = ft_substr(pwd, 0, temp - pwd);
		free (pwd);
		pwd = temp;
	}
	return (pwd);
}

//line 45 error mem, exit prog
static char	*add_path_to_pwd(char *pwd, char *dir)
{
	char	*temp;

	temp = pwd;
	pwd = ft_join_strs(pwd, "/", dir);
	if (pwd == NULL)
		return (free(temp), NULL);
	free(temp);
	return (pwd);
}

static char	*get_current_pwd(t_read_input *in)
{
	char	*pwd;
	t_dlist	*l;

	l = mini_getenv("PWD", in->env);
	if (l->status > CHANGED_CD)
		pwd = ft_strdup(l->backup);
	else
		pwd = ft_strdup(l->value);
	return (pwd);
}

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
				return (ft_free_split(dirs), NULL);
		}
		i++;
	}
	return (ft_free_split(dirs), pwd);
}
