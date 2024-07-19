/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:04:28 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/19 20:51:49 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief	gets the value of an environment variable in the envp matrix
 * @param	char * var name of the variable whose value is to be gotten
 * @param	t_dlist	*env pointer to first element of the list with env variables
 * @return	char* pointer to the value or NULL if it doesn't exist
 */
char	*mini_getenv_value(char *key, t_dlist *env)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, ft_strlen(key)))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

t_dlist	*mini_getenv(char *key, t_dlist	*env)
{
	size_t	len;

	len = ft_strlen(key);
	while (env)
	{
		if (!ft_strncmp(env->key, key, len) && ft_strlen(env->key) == len)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	mini_replace_env(t_dlist *l, char *value, int status)
{
	if (l->status == CHANGED_UNSET)
		return ;
	if (l->value)
		free(l->value);
	l->value = ft_strdup(value);
	l->status = status;
}

/**
 * @brief	sets an environment variable in the env list (adds it at the end of 
 * 			it if the variable didn't exist)
 * @param	char * var name of the variable to set (must be 0 ended)
 * @param	char * val value to assign to the variable (must be 0 ended)
 * @param	t_dlist	**env pter to first element of the list with env variables
 * @return	int 0 if the variable was set, 1 if it was updated
 */
int	mini_setenv(char *var, char *val, int status, t_dlist **env)
{
	char	*value;
	char	*key;
	t_dlist	*start;

	start = mini_getenv(var, *env);
	if (start)
		return (mini_replace_env(start, val, status), 1);
	else if (start == NULL && !ft_strncmp(var, "OLDPWD", 6))
		return (OK);
	value = ft_strdup(val);
	if (value == NULL)
		return (ERROR);
	key = ft_strdup(var);
	if (key == NULL)
		return (free(value), ERROR);
	start = ft_dlistnew(key, value);
	if (start == NULL)
		return (free(value), free(key), ERROR);
	return (ft_dlistadd_back(env, start), OK);
}

/**
 * @brief	makes a matrix of strings from a list of strings 
 * @param	t_dlist	*env first list element
 * @return	char** matrix
 */
char	**makes_env_matrix_from_env_list(t_dlist	*env)
{
	char	**env_matrix;
	size_t	list_length;
	size_t	i;
	t_dlist	*next;

	next = env;
	list_length = 0;
	while (next)
	{
		if (next->value)
			list_length++;
		next = next->next;
	}
	env_matrix = (char **)malloc(sizeof(char *) * (list_length + 1));
	if (!env_matrix)
		return (NULL);
	env_matrix[list_length] = NULL;
	i = 0;
	while (i < list_length)
	{
		if (env->value)
			env_matrix[i++] = ft_join_strs(env->key, "=", env->value);
		env = env->next;
	}
	return (env_matrix);
}
