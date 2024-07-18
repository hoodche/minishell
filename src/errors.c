/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:29:28 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 21:50:06 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief	prints to stderr a specific message for the error type received as
 * 			argument
 * @param	t_mini_error err_type
 */
void	mini_perror(t_mini_error err_type)
{
	int	copy_fd;

	copy_fd = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	if (err_type == UNB_SING_QUOT)
		printf("%s: unclosed \'\n", ERR_SYNTAX);
	else if (err_type == UNB_DOUB_QUOT)
		printf("%s: unclosed \"\n", ERR_SYNTAX);
	else if (err_type == CLOBBER)
		printf("%s: >| operator not accepted\n", ERR_SYNTAX);
	else if (err_type == OR)
		printf("%s: || operator not accepted\n", ERR_SYNTAX);
	else if (err_type == PIPE_SIGN)
		printf("%s near unexpected token `|'\n", ERR_SYNTAX);
	else if (err_type == UNEXPECTED_NL)
		printf("%s near unexpected token 'newline'\n", ERR_SYNTAX);
	dup2(copy_fd, STDOUT_FILENO);
	close(copy_fd);
}

void	mini_program_error(t_read_input *in)
{
	perror("minishell: program error");
	g_status = EXIT_FAILURE;
	mini_destroy_and_exit(in);
}

void	mini_errno_error(int status, char *cmd, char *arg)
{
	int	copy_fd;

	copy_fd = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	g_status = status;
	if (arg)
		printf("minishell: %s: %s: %s\n", cmd, arg, strerror(errno));
	else
		printf("minishell: %s: %s\n", cmd, strerror(errno));
	dup2(copy_fd, STDOUT_FILENO);
	close(copy_fd);
}

void	mini_formatted_error(int status, char *cmd, char *arg, char *msg)
{
	int	copy_fd;

	copy_fd = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	g_status = status;
	if (arg)
		printf("minishell: %s: %s: %s\n", cmd, arg, msg);
	else
		printf("minishell: %s: %s\n", cmd, msg);
	dup2(copy_fd, STDOUT_FILENO);
	close(copy_fd);
}

void	error_execve(t_read_input *in, t_cmd *cmd, char **envs)
{
	char	*msg;

	write(2, cmd->argv_for_execve[0], ft_strlen(cmd->argv_for_execve[0]));
	write(2, ": ", 2);
	if (ft_strchr(cmd->argv_for_execve[0], '/') != NULL)
	{
		msg = strerror(errno);
		write(2, msg, ft_strlen(msg));
	}
	else
		write(2, CMD_NOT_FOUND, ft_strlen(CMD_NOT_FOUND));
	write(2, "\n", 1);
	g_status = 127;
	free_matrix(envs);
	mini_destroy_and_exit(in);
}
