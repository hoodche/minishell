/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:48:55 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/18 20:47:09 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* rl_replace_line("", 0): Clears the current input line in the Readline buffer.
rl_on_new_line: Informs Readline that the cursor is now on a new line.
The rl_on_new_line function is crucial for maintaining the consistency of the 
Readline library's state, especially after an interruption like SIGINT.
It ensures that the command prompt is correctly positioned and ready for
 further user input
*/
// exit status for SIGINT is ([signal received] 128 + 2 [SIGINT value])
//ioctl(STDIN_FILENO, TIOCSTI, "\n") escribe en stdin pero OJO, tb lo imprime en
// stdout
// rl_replace_line("", 0); vacía rl_line_buffer, el buffer donde readline() 
// almacena lo que se teclea
void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130 ;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}
