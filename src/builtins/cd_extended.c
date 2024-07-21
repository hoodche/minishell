/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_extended.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:31:54 by gtaza-ca          #+#    #+#             */
/*   Updated: 2024/07/21 15:15:09 by gtaza-ca         ###   ########.fr       */
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

	if (ft_strlen(pwd) == 1 && !ft_strncmp(pwd, "/", 1))
	{
		free(pwd);
		pwd = ft_strdup("");
	}
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

static char	*check_pwd_empty(char *pwd)
{
	if (ft_strlen(pwd) == 0)
	{
		free(pwd);
		pwd = ft_strdup("/");
		return (pwd);
	}
	return (pwd);
}
