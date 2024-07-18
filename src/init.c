/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:24:06 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 20:40:54 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	update_pwd_env(t_read_input	*in)
{
	t_dlist	*l;

	l = mini_getenv("PWD", in->env);
	if (l == NULL)
		return ;
	if (l->value)
		l->backup = ft_strdup(l->value);
}

/**
 * @brief	initializes environment variables list by making a copy of the 
 * 			global environ received as third arg envv in main. Then updates env 
 * 			var SHLVL by incrementing it (+1)
 * @param	t_read_input	*input struct to be filled with envs
 * @param	char** envv global environ variable received in main as third arg
 */
void	init_environ(t_read_input	*input, char **envv)
{
	char	*aux;
	char	*key;
	char	*value;

	input->env = NULL;
	while (*envv)
	{
		value = ft_strchr(*envv, '=');
		key = ft_substr(*envv, 0, value - *envv);
		if (key == NULL)
			return ;
		mini_setenv(key, value + 1, ORIGINAL, &input->env);
		free(key);
		envv++;
	}
	aux = mini_getenv_value("SHLVL", input->env);
	if (aux)
	{
		aux = ft_itoa(ft_atoi(aux) + 1);
		mini_setenv("SHLVL", aux, ORIGINAL, &input->env);
		free(aux);
	}
	else
		mini_setenv("SHLVL", "1", ORIGINAL, &input->env);
	update_pwd_env(input);
}

/**
 * @brief	initializes struct to 0 
 * @param	t_read_input	*input struct to be filled
 */
void	reset_data(t_read_input	*input)
{
	free_resources(input);
	ft_bzero(input->ruling_dbl_quotes, sizeof(input->ruling_dbl_quotes));
	ft_bzero(input->ruling_sgl_quotes, sizeof(input->ruling_sgl_quotes));
	ft_bzero(input->redirec_ptr, sizeof(input->redirec_ptr));
	ft_bzero(input->pipes, sizeof(input->pipes));
	ft_bzero(input->cmds, sizeof(input->cmds));
	input->typed = NULL;
	input->pipe_fd = NULL;
	input->cmd_count = 0;
	input->heredoc_count = 0;
	input->pipe_count = 0;
}

/**
 * @brief	initializes the prompt string and reads input from prompt 
 * @param	t_read_input	*input struct 
 */
void	init_prompt(t_read_input	*input)
{
	input->typed = readline("\033[1;35m""grace&nachoMinishell$""\033[0m");
}

/**
 * @brief	initializes struct to 0 
 * @param	t_read_input	*input struct to be filled
 */
void	minishell_init(t_read_input	*input, char **envv)
{
	ft_bzero(input->ruling_dbl_quotes, sizeof(input->ruling_dbl_quotes));
	ft_bzero(input->ruling_sgl_quotes, sizeof(input->ruling_sgl_quotes));
	ft_bzero(input->redirec_ptr, sizeof(input->redirec_ptr));
	ft_bzero(input->pipes, sizeof(input->pipes));
	ft_bzero(input->cmds, sizeof(input->cmds));
	input->typed = NULL;
	input->pipe_fd = NULL;
	input->cmd_paths = NULL;
	input->cmd_count = 0;
	input->heredoc_count = 0;
	input->pipe_count = 0;
	init_environ(input, envv);
}
