/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:01:15 by igcastil          #+#    #+#             */
/*   Updated: 2024/06/30 12:53:17 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief	prints report with input struct data
 * @param	t_read_input	*input struct
 * @param	int i index in the input->cmds array of the command to print
 */
void	print_cmd_args_for_execve(t_read_input *input, int i)
{
	int	j;

	j = -1;
	printf("THIS CMD argv_for_execve  ---------->>>>>>\n");
	while (input->cmds[i].argv_for_execve[++j])
		printf("input->cmds[%d].argv_for_execve[%d] is #%s# \n", i, j,
			input->cmds[i].argv_for_execve[j]);
}

/**
 * @brief	prints report with input struct data
 * @param	t_read_input	*input struct
 * @param	int i index in the input->cmds array of the command to print
 */
void	print_ruling_double_quotes_pointers(t_read_input *input, int i)
{
	int	j;

	printf("cmd %d ruling_dbl_quotes are in:\n", i + 1);
	j = -1;
	while (input->cmds[i].ruling_dbl_quotes[++j])
		printf("%p \n", input->cmds[i].ruling_dbl_quotes[j]);
}

/**
 * @brief	prints report with input struct data
 * @param	t_read_input	*input struct
 * @param	int i index in the input->cmds array of the command to print
 */
void	print_debug_log_cmds_redirections_array(t_read_input *input, int i)
{
	int	j;

	printf("THIS CMD REDIRECTIONS ---------->>>>>>\n");
	j = -1;
	while (input->cmds[i].redir[++j].kind)
	{
		printf("input->cmds[i].redir[j].kind is #%d# \n",
			input->cmds[i].redir[j].kind);
		printf("input->cmds[i].redir[j].fd #%d# \n",
			input->cmds[i].redir[j].fd);
		printf("input->cmds[i].redir[j].filename is #%s# \n",
			input->cmds[i].redir[j].filename);
		printf("input->cmds[i].redir[j].delimeter is #%s# \n",
			input->cmds[i].redir[j].delimeter);
		printf("input->cmds[i].redir[j].expand_heredoc_content is #%d# \n",
			input->cmds[i].redir[j].expand_heredoc_content);
	}
}

/**
 * @brief	prints report with input struct data
 * @param	t_read_input	*input struct
 */
void	print_debug_log_cmds_array(t_read_input	*input)
{
	int	i;
	int	j;

	printf("-----------ITERATE CMDS ARRAY----------\n");
	i = 0;
	while (i < input->cmd_count)
	{
		printf("cmd %d str is >%s< in %p\n", i + 1, input->cmds[i].str,
			input->cmds[i].str);
		print_ruling_double_quotes_pointers(input, i);
		printf("cmd %d ruling_sgl_quotes are in:\n", i + 1);
		j = -1;
		while (input->cmds[i].ruling_sgl_quotes[++j])
			printf("%p \n", input->cmds[i].ruling_sgl_quotes[j]);
		printf("cmd %d redirec_ptr are in:\n", i + 1);
		j = -1;
		while (input->cmds[i].redirec_ptr[++j])
			printf("%p \n", input->cmds[i].redirec_ptr[j]);
		print_debug_log_cmds_redirections_array(input, i);
		print_cmd_args_for_execve(input, i);
		i++;
	}
}

/**
 * @brief	prints report with input struct data
 * @param	t_read_input	*input struct
 */
void	print_debug_log(t_read_input	*input)
{
	int	i;

	printf("------IN PROMPT STRUCT---\ninput->typed is in %p:\n", input->typed);
	printf("dbl quotes are in:\n");
	i = -1;
	while (input->ruling_dbl_quotes[++i])
		printf("%p \n", input->ruling_dbl_quotes[i]);
	printf("sgl quotes are in:\n");
	i = -1;
	while (input->ruling_sgl_quotes[++i])
		printf("%p \n", input->ruling_sgl_quotes[i]);
	printf("redirec_ptr are in:\n");
	i = -1;
	while (input->redirec_ptr[++i])
		printf("%p \n", input->redirec_ptr[i]);
	printf("cmd_count is %d \n", input->cmd_count);
	printf("cmds are, from last to first:\n");
	i = input->cmd_count;
	while (--i >= 0)
		printf("%s \n", input->pipes[i]);
	print_debug_log_cmds_array(input);
}
