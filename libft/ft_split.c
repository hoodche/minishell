/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 17:12:52 by igcastil          #+#    #+#             */
/*   Updated: 2023/10/02 23:52:36 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief	Allocates (with malloc(3)) and returns an array of strings obtained
 * 			by splitting ’s’ using the character ’c’ as a delimiter. The array
 * 			must be ended by a NULL pointer.
 * @param	char const * string to be splitted
 * @param	char c * delimiter character.
 * @return	char ** array of new strings resulting from the split. NULL if the 
 * 			allocation fails
 */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	count;
	size_t	is_inside_word;

	count = 0;
	is_inside_word = 0;
	while (*s)
	{
		if (*s != c)
		{
			if (!is_inside_word)
			{
				is_inside_word = 1;
				count++;
			}
		}
		else
			is_inside_word = 0;
		s++;
	}
	return (count);
}

static char	*extract_word(const char **s, char c)
{
	const char	*start;
	char		*word;
	size_t		word_length;
	size_t		i;

	start = *s;
	while (**s && **s != c)
		(*s)++;
	word_length = *s - start;
	word = (char *)malloc(word_length + 1);
	if (!word)
		return (0);
	i = 0;
	while (i < word_length)
	{
		word[i] = start[i];
		i++;
	}
	word[word_length] = '\0';
	if (**s)
		(*s)++;
	return (word);
}

static char	**free_memory(char **strs_arr, size_t elements)
{
	while (elements != 0)
	{
		free(strs_arr[elements - 1]);
		elements--;
	}
	free(strs_arr);
	return (0);
}

char	**ft_split(char const *s, char c)
{
	size_t	index;
	char	*word ;
	char	**str_split;

	if (!s)
		return (0);
	str_split = (char **)malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!str_split)
		return (0);
	index = 0;
	while (*s)
	{
		if (*s != c)
		{
			word = extract_word(&s, c);
			if (!word)
				return (free_memory(str_split, index));
			str_split[index] = word;
			index++;
		}
		else
			s++;
	}
	str_split[index] = 0;
	return (str_split);
}

/* #include <stdio.h>
int main() {
    const char *input = "hello world how are you";
    char **result = ft_split(input, ' ');

    if (result) {
        for (size_t i = 0; result[i]; i++) {
            printf("%s\n", result[i]);
            free(result[i]);
        }
        free(result);
    } else {
        printf("Memory allocation failed.\n");
    }

    return 0;
} */
