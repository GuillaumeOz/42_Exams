/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gozsertt <gozsertt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 14:33:08 by gozsertt          #+#    #+#             */
/*   Updated: 2021/07/02 12:52:33 by gozsertt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct	s_zone
{
	int     width;
	int     height;
	char	background;
}				t_zone;

typedef struct	s_shape
{
	char	type;
	float	x;
	float	y;
	float	rad;
	char	color;
}				t_shape;

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	error_msg(char *msg)
{
	write(1, msg, ft_strlen(msg));
}

int	create_zone(FILE *file, t_zone *zone)
{
	int fscanf_ret;

	fscanf_ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background);
	if (fscanf_ret != 3)
		return (1);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (1);
	return (0);
}

char	*malloc_area(t_zone *zone)
{
	char *area;
	int i;

	area = (char *)malloc(sizeof(char) * (zone->height * zone->width));
	if (area == NULL)
		return (NULL);
	i = 0;
	while (i < (zone->height * zone->width))
	{
		area[i] = zone->background;
		i++;
	}
	return (area);
}

float calc_dist(float xa, float ya, float xb, float yb)
{
	float x_dist;
	float y_dist;
	float ret;

	x_dist = (xa - xb) * (xa - xb);
	y_dist = (ya - yb) * (ya - yb);
	ret = sqrtf(x_dist + y_dist);
	return (ret);
}

void	full_circle(t_zone *zone, t_shape *shape, char *area)
{
	int x;
	int y;
	float dist;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			dist = calc_dist(x, y, shape->x, shape->y);
			dist = dist - shape->rad;
			if (dist <= 0.00000000)
				area[x + (y * zone->width)] = shape->color;
			x++;
		}
		y++;
	}
}

void	empty_circle(t_zone *zone, t_shape *shape, char *area)
{
	int x;
	int y;
	float dist;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			dist = calc_dist(x, y, shape->x, shape->y);
			dist = dist - shape->rad;
			if (dist <= 0.00000000)
				if (dist > -1.00000000)
					area[x + (y * zone->width)] = shape->color;
			x++;
		}
		y++;
	}
}

int	create_shape(FILE *file, t_zone *zone, t_shape *shape, char *area)
{
	int fscanf_ret;

	while ((fscanf_ret = fscanf(file, "%c %f %f %f %c\n", &shape->type, &shape->x, &shape->y, &shape->rad, &shape->color)) == 5)
	{
		if (shape->rad <= 0.00000000)
			return (1);
		if (shape->type != 'c' && shape->type != 'C')
			return (1);
		if (shape->type == 'c')
			empty_circle(zone, shape, area);
		else if (shape->type == 'C')
			full_circle(zone, shape, area);
	}
	if (fscanf_ret == -1)
		return (0);
	return (1);
}

void	print_area(t_zone *zone, char *area)
{
	int i;

	i = 0;
	while (i < zone->height)
	{
		write(1, area + (i * zone->width), zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	FILE	*file;
	t_zone 	zone;
	t_shape	shape;
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
		error_msg("Error: Operation file corrupted\n");
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
	free(area);
	fclose(file);
	return (0);
}
