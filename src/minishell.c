/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:12:26 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 21:06:30 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_status;

int	non_interactive_minishell(char **argv, t_read_input	*input)
{
	reset_data(input);
	input->typed = ft_strdup(argv[2]);
	if (is_valid_input(input->typed) != 1)
		return (1);
	if (!are_input_quotes_balanced(input->typed))
	{
		free(input->typed);
		return (mini_perror(UNB_DOUB_QUOT), g_status = 127, 127);
	}
	else
		locate_input_quotes(input);
	if (!parse(input))
		mini_destroy_and_exit(input);
	if (DEBUG_MODE)
		print_debug_log(input);
	mini_select_execution(input);
	mini_destroy_and_exit(input);
	return (g_status);
}

//line 42 CTRL \ is ignored
//line 48 controls ctrl + D
void	interactive_minishell(t_read_input	*input)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
	while (1)
	{
		reset_data(input);
		init_prompt(input);
		if (input->typed == NULL)
		{
			printf("exit\n");
			g_status = 0;
			mini_destroy_and_exit(input);
		}
		if (!is_valid_input(input->typed))
			continue ;
		if (!are_input_quotes_balanced(input->typed))
			continue ;
		else
			locate_input_quotes(input);
		if (!parse(input))
			continue ;
		if (DEBUG_MODE)
			print_debug_log(input);
		if (mini_select_execution(input) == ERROR)
			continue ;
	}
}

int	main(int argc, char **argv, char **envv)
{
	t_read_input	input;

	g_status = 0;
	minishell_init(&input, envv);
	if (argc == 3 && !ft_strncmp(argv[1], "-c", ft_strlen(argv[1])))
		return (non_interactive_minishell(argv, &input));
	else if (argc >= 3 && !ft_strncmp(argv[1], "-c", ft_strlen(argv[1])))
	{
		printf("minishell: non interactive(-c) mode admits only 3 arguments\n");
		mini_destroy_and_exit(&input);
		return (1);
	}
	else
		interactive_minishell(&input);
	exit(g_status);
}
