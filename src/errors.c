/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:29:28 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/20 16:49:04 by gtaza-ca         ###   ########.fr       */
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

void	error_execve(t_read_input *in, t_cmd *cmd)
{
	char	*s_cmd;

	s_cmd = cmd->argv_for_execve[0];
	if (!ft_strncmp(s_cmd, "./", 2) && ft_strlen(s_cmd) == 2)
		mini_formatted_error(126, s_cmd, NULL, ERR_ISDIR);
	else if (ft_strchr(s_cmd, '/') != NULL)
	{
		if (errno == ENOENT)
			mini_formatted_error(127, s_cmd, NULL, strerror(errno));
		else
			mini_formatted_error(126, s_cmd, NULL, strerror(errno));
	}
	else
		mini_formatted_error(127, s_cmd, NULL, CMD_NOT_FOUND);
	mini_destroy_and_exit(in);
}
