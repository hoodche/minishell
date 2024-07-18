/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 19:50:13 by igcastil          #+#    #+#             */
/*   Updated: 2024/01/29 09:50:57 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_last_line(char **new_line, char **remainder_addr)
{
	*new_line = ft_strdup_gnl(*remainder_addr);
	if (!(*new_line))
		return (NULL);
	**remainder_addr = '\0';
	return (*new_line);
}

/**
 * @brief	updates remainder by moving pointer after '\n'
 * @param	char ** address of pointer to text read from file so far
 * @return	char * pointer to line to be returned.
 */
char	*trim_remainder(char	**remainder_addr)
{
	char	*new_line;
	char	*aux;
	int		i;

	if ((ft_strchr_gnl(*remainder_addr, '\n')) == NULL)
		return (get_last_line(&new_line, remainder_addr));
	i = 0;
	while ((*remainder_addr)[i] != '\n')
		i++;
	new_line = malloc(sizeof(char) * (i + 2));
	if (!new_line)
		return (NULL);
	new_line[i + 1] = '\0';
	while (i >= 0)
	{
		new_line[i] = (*remainder_addr)[i];
		i--;
	}
	aux = ft_strchr_gnl(*remainder_addr, '\n');
	aux++;
	aux = ft_strdup_gnl(aux);
	free(*remainder_addr);
	*remainder_addr = aux;
	return (new_line);
}

int	ft_read_fd(int fd, char **buffer, char **remainder, int *read_bytes_count)
{
	char	*aux;

	*read_bytes_count = read(fd, *buffer, BUFFER_SIZE);
	if (*read_bytes_count == -1)
		return (free(*remainder), free(*buffer), 0);
	(*buffer)[*read_bytes_count] = '\0';
	aux = ft_strjoin_gnl(*remainder, *buffer);
	if (!aux)
		return (free(*remainder), free(*buffer), 0);
	free(*remainder);
	*remainder = aux;
	return (1);
}

/**
 * @brief	reads from fd into remainder (only if there is no '\n' in remainder)
 * @param	int fd file descriptor to read from
 * @param	char * text read from file so far
 * @return	char * text read from file so far
 */
char	*read_from_fd_to_remainder(int fd, char *remainder)
{
	char	*buffer;
	int		read_bytes_count;

	if (!remainder)
	{
		remainder = malloc (sizeof(char) * (BUFFER_SIZE + 1));
		if (!remainder)
			return (NULL);
		*remainder = '\0';
	}
	if (ft_strchr_gnl(remainder, '\n'))
		return (remainder);
	buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (free (remainder), NULL);
	read_bytes_count = 1;
	while (!ft_strchr_gnl(remainder, '\n') && read_bytes_count > 0)
	{
		if (!ft_read_fd(fd, &buffer, &remainder, &read_bytes_count))
			return (NULL);
	}
	free(buffer);
	return (remainder);
}

/**
 * @brief	gets line from fd file
 * @param	int fd file descriptor to read from
 * @return	char * pointer to the line. NULL if error or EOF
 */

char	*get_next_line(int fd)
{
	static char	*remainder[MAX_SIZE / sizeof (char *)];
	char		*new_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	remainder[fd] = read_from_fd_to_remainder(fd, remainder[fd]);
	if (!remainder[fd])
		return (NULL);
	if (!(*(remainder[fd])))
		return (free(remainder[fd]), remainder[fd] = NULL, NULL);
	new_line = trim_remainder(&remainder[fd]);
	if (!new_line)
		return (free(remainder[fd]), remainder[fd] = NULL, NULL);
	return (new_line);
}
/* #include <stdio.h>
int main()
{
	char *message = "que pacha cara huevo\n";
	write(STDIN_FILENO, message, ft_strlen_gnl(message));
	printf("%s", get_next_line(STDIN_FILENO));
	return 0;
} */