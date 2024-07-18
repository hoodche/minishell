/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 22:44:49 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/17 12:33:40 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief	sets minishell´s prompt to "user@host:pwd^". Checks if those 3 are 
 * 			variables set in the environment. If not, sets them to "unknown" (to
 * 			avoid segfault in ft_strlen(NULL))
 * @param	char prompt[PROMPT_SIZE] string to be filled with prompt
 */
void	set_prompt(char prompt[PROMPT_SIZE])
{
	ft_bzero(prompt, PROMPT_SIZE);
	if (getenv("USER"))
		ft_strlcat(prompt, getenv("USER"),
			ft_strlen(prompt) + ft_strlen(getenv("USER")) + 1);
	else
		ft_strlcat(prompt, "unknown_user",
			ft_strlen(prompt) + ft_strlen("unknown_user") + 1);
	ft_strlcat(prompt, "@", ft_strlen(prompt) + 1 + 1);
	if (getenv("NAME"))
		ft_strlcat(prompt, getenv("NAME"),
			ft_strlen(prompt) + ft_strlen(getenv("NAME")) + 1);
	else
		ft_strlcat(prompt, "unknown_host",
			ft_strlen(prompt) + ft_strlen("unknown_host") + 1);
	ft_strlcat(prompt, ":", ft_strlen(prompt) + 1 + 1);
	if (getenv("PWD"))
		ft_strlcat(prompt, getenv("PWD"),
			ft_strlen(prompt) + ft_strlen(getenv("PWD")) + 1);
	else
		ft_strlcat(prompt, "unknown_pwd",
			ft_strlen(prompt) + ft_strlen("unknown_pwd") + 1);
	ft_strlcat(prompt, "^", ft_strlen(prompt) + 1 + 1);
}

/**
 * @brief	checks if input_str typed at prompt:
 * 			1) could not be read by readline (!NULL)
 * 			2) is empty
 * 			3) is a comment
 * @param	char *input_str char stream typed at prompt (\n removed by readline)
 * @return	int 1 if input_str must be parsed. 0 in cases 1,2,3 (minishell will 
 * 			print prompt again. Cases 1 and 2 will not add input_str to history)
 */
int	is_valid_input(char *input_str)
{
	if (!input_str)
		return (-1);
	if (!(*input_str))
		return (0);
	add_history(input_str);
	if (input_str[0] == '#')
		return (0);
	return (1);
}
