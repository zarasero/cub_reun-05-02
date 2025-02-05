/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:01:10 by morgane           #+#    #+#             */
/*   Updated: 2025/02/04 18:33:09 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void color_is_valid(t_data *data)
{
	int i;

	i = 0;
	if (data->c_color)
	{
		while (data->c_color[i])
		{
			if (data->c_color[i] > 255)
				err(RGB_SUP);
			i++;
		}
	}
	i = 0;
	if (data->f_color)
	{
		while (data->f_color[i])
		{
			if (data->f_color[i] > 255)
				err(RGB_SUP);
			i++;
		}
	}
	return;
}

void print_char_tab(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
	return;
}

void	err(char *str)
{
	while (*str)
		write(2, str++, 1);
	exit(1);
}

int count_lines_fd(char *argv)
{
	char *line;
	int len;
	int fd;

	line = NULL;
	len = 0;
	fd = open(argv, O_RDONLY);
	if (fd < 0)
		err(OPENFD);
	while ((line = get_next_line(fd)) != NULL)
	{
		len++;
		free(line);
	}
	close(fd);
	return (len);
}

int map_line_max_lenght(char **map)
{
	int i;
	size_t max;
	size_t len;

	i = 0;
	max = 0;
	len = 0;
	while (map[i])
	{
		len = ft_strlen(map[i]);
		if (len > max)
			max = len;
		i++;
	}
	return ((int)max);
}

bool	is_valid_char(char c)
{
	return (c == '0' || c == '1' || c == 'S' || c == 'W'
		|| c == 'E' || c == 'N');
}
