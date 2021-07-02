/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gozsertt <gozsertt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/03 12:01:22 by gozsertt          #+#    #+#             */
/*   Updated: 2021/07/02 12:55:34 by gozsertt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_zone
{
	int     width;
	int     height;
	char    background;
}               t_zone;

typedef struct s_shape
{
	char	type;
	float	x;
	float	y;
	float 	width;
	float	height;
	char	color;
}				t_shape;

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

void	error_msg(char *msg)
{
	write(1, msg, ft_strlen(msg));
}

int create_zone(FILE *file, t_zone *zone)
{
	int fscanf_ret;

	fscanf_ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background);
	if (fscanf_ret != 3)
		return (1);
	if (zone->width <= 0 || zone->height <= 0
		|| zone->width > 300 || zone->height > 300)
		return (1);
	return (0); 
}

char *malloc_area(t_zone *zone)
{
	char *area;
	int i;

	area = (char*)malloc(sizeof(char) * zone->width * zone->height);
	if (area == NULL)
		return (NULL);
	i = 0;
	while (i < zone->height * zone->width)
	{
		area[i] = zone->background;
		i++;
	}
	return (area);
}

void	empty_rectangle(t_zone *zone, t_shape *shape, char *area)
{
	int x;
	int y;

	y = 0;
	while(y < zone->height)
	{
		x = 0;
		while(x < zone->width)
		{
			if (shape->x <= x && shape->y <= y && (shape->x + shape->width) >= x
				&& shape->y <= y && (shape->y + shape->height) >= y)
				if (shape->x + 1.00000000 > x || shape->y + 1.00000000 > y
				|| (shape->x + shape->width) - 1.00000000 < x || (shape->y + shape->height) - 1.00000000 < y)
						area[x + (y * zone->width)] = shape->color;
			x++;
		}
		y++;
	}
}

void	full_rectangle(t_zone *zone, t_shape *shape, char *area)
{
	int x;
	int y;

	y = 0;
	while(y < zone->height)
	{
		x = 0;
		while(x < zone->width)
		{
			if (shape->x <= x && shape->y <= y && (shape->x + shape->width) >= x
				&& shape->y <= y && (shape->y + shape->height) >= y)
				area[x + (y * zone->width)] = shape->color;
			x++;
		}
		y++;
	}
}

int create_shape(FILE *file, t_zone *zone, t_shape *shape, char *area)
{
	int fscanf_ret;

	while ((fscanf_ret = fscanf(file, "%c %f %f %f %f %c\n", &shape->type, &shape->x, &shape->y, &shape->width, &shape->height, &shape->color)) == 6)
	{
		if (shape->width <= 0 || shape->height <= 0)
			return (1);
		if (shape->type != 'r' && shape->type != 'R')
			return (1);
		if (shape->type == 'r')
			empty_rectangle(zone, shape, area);
		else if (shape->type == 'R')
			full_rectangle(zone, shape, area);
	}
	if (fscanf_ret == -1)
		return (0);
	return (1);
}

void	print_area(t_zone *zone, char *area)
{
	int i;

	i = 0;
	while(i < zone->height)
	{
		write (1, area + (i * zone->width), zone->width);
		write (1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	FILE	*file;
	t_zone	zone;
	t_shape shape;
	char	*area;

	if (argc != 2)
	{
		error_msg("Error: argument\n");
		return (1);
	}
	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		error_msg("Error: Operation file corrupted\n");
		return (1);
	}
	if (create_zone(file, &zone) == 1)
	{
		error_msg("Error: Operation file corrupted\n");
		fclose(file);
		return (1);
	}
	area = malloc_area(&zone);
	if (area == NULL)
	{
		fclose(file);
		return (1);
	}
	if (create_shape(file, &zone, &shape, area) == 1)
	{
		error_msg("Error: Operation file corrupted\n");
		free(area);
		fclose(file);
		return (1);
	}
	print_area(&zone, area);
	fclose(file);
	free(area);
	return (0);
}
