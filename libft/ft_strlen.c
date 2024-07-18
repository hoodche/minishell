/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 11:09:33 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/10 20:16:52 by gtaza-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief	calculate the length of a string excluding terminating null byte 
 * 			('\0')
 * @param	char* string whose length is to be measured. Segfault if NULL, just
 * 			like strlen from string.h * 
 * @return	size_t number of bytes in the string
 * @libc	<string.h>
 */

// size_t is an unsigned integral data type which is defined in various header
// files such as:<stddef.h>, <stdio.h>, <stdlib.h>, <string.h>, <time.h>, ...
#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	strlen;

	strlen = 0;
	if (s == NULL)
		return (strlen);
	while (s[strlen] != 0)
		strlen++;
	return (strlen);
}
