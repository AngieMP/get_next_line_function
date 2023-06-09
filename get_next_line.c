/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angmedin <angmedin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 11:31:19 by angmedin          #+#    #+#             */
/*   Updated: 2023/04/18 14:48:22 by angmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*nolinebreak(char *whole_line)
{
	if (ft_strlen(whole_line) == 0)
	{
		free(whole_line);
		return (NULL);
	}
	else
		return (whole_line);
}

static char	*ft_line_and_rest(char **line)
{
	char	*rest;
	char	*whole_line;
	int		i;

	i = 0;
	if (*line == NULL)
		return (NULL);
	while ((*line)[i] != '\n' && (*line)[i])
		i++;
	if ((*line)[i] == '\n')
	{
		whole_line = ft_substr(*line, 0, i + 1);
		rest = ft_strdup(*line + (i + 1));
		free(*line);
		*line = rest;
		return (whole_line);
	}
	whole_line = ft_strdup(*line);
	free(*line);
	*line = NULL;
	return (nolinebreak(whole_line));
}

static char	*rnsend(int read_bytes, int fd, char *buffer, char **line)
{
	char	*temp;

	while (read_bytes > 0)
	{
		buffer[read_bytes] = '\0';
		if (*line == NULL)
			*line = ft_strdup("");
		temp = ft_strjoin(*line, buffer);
		free(*line);
		*line = temp;
		if (ft_strchr(buffer, '\n'))
			break ;
		read_bytes = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (ft_line_and_rest(line));
}

char	*get_next_line(int fd)
{
	char		*buffer;
	static char	*line = NULL;
	int			read_bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * (sizeof(char)));
	if (!buffer)
		return (NULL);
	read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (read_bytes < 0)
	{
		free(buffer);
		free(line);
		line = NULL;
		return (NULL);
	}
	return (rnsend(read_bytes, fd, buffer, &line));
}

/*#include <stdio.h>
int	main()
{
	char	*line = NULL;
	int		fd1;
	fd1 = open("text1.txt", O_RDONLY);
	//fd2 = open("text2.txt", O_RDONLY);
	//fd3 = open("text3.txt", O_RDONLY);
	if (fd1 == -1 )
	{
		printf("Error al abrir el archivo\n");
		return (1);
	}
	while ((line = get_next_line(fd1)))
	{
		printf("%s", line);
		free(line);
	}
	close(fd1);
	system("leaks -q a.out");
	return (0);
}*/
