/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gozsertt <gozsertt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 14:33:08 by gozsertt          #+#    #+#             */
/*   Updated: 2020/09/01 18:50:42 by gozsertt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct s_zone
{
	int		width;
	int		height;
	char	background;
}		t_zone;

typedef struct s_shape
{
	char	type;
	float	x;
	float	y;
	float	rad;
	char	color;
}		t_shape;

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int clear_all(FILE *file, char *drawing)
{
	fclose(file);
	if (!drawing)
		free(drawing);
	return (1);
}

int error_msg(char *msg)
{
	write(1, msg, ft_strlen(msg));
	return(1);
}

int get_zone(FILE *file, t_zone *zone)
{
	int scanf_ret;

	if ((scanf_ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return (0);
	if (zone->width <= 0 || zone->height <= 0 || zone->width > 300 || zone->height > 300)
		return (0);
	if (scanf_ret == -1)
		return (0);
	return (1);
}

char	*malloc_drawing(t_zone *zone)
{
	char *drawing;
	int i;

	if (!(drawing = (char*)malloc(sizeof(char) * (zone->width * zone->height))))
		return (NULL);
	i = 0;
	while (i < zone->height * zone->width)
		drawing[i++] = zone->background;
	return (drawing);
}

void empty_circle(char *drawing, t_zone *zone, t_shape *shape)
{
	int y;
	int x;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			if (sqrtf(powf((shape->x - x), 2) + powf((shape->y - y), 2)) - shape->rad <= 0 
				&& sqrtf(powf((shape->x - x), 2) + powf((shape->y - y), 2)) - shape->rad > -1)
				drawing[(y * zone->width) + x] = shape->color;
			x++;
		}
		y++;
	}	
}

void full_circle(char *drawing, t_zone *zone, t_shape *shape)
{
	int y;
	int x;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			if (sqrtf(powf((shape->x - x), 2) + powf((shape->y - y), 2)) - shape->rad <= 0)
				drawing[(y * zone->width) + x] = shape->color;
			x++;
		}
		y++;
	}	
}

int get_shapes(FILE *file, t_zone *zone, char *drawing)
{
	t_shape tmp;
	int		scanf_ret;

	while ((scanf_ret = fscanf(file, "%c %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.rad, &tmp.color)) == 5)
	{
		if (tmp.rad <= 0 || ( tmp.type != 'c' && tmp.type != 'C'))
			return (0);
		if (tmp.type == 'c')
			empty_circle(drawing, zone, &tmp);
		if (tmp.type == 'C')
			full_circle(drawing, zone, &tmp);
	}
	if (scanf_ret != -1)
		return (0);
	return (1);
}

void draw_drawing(t_zone *zone, char *drawing)
{
	int i;

	i = 0;
	while (i < zone->height)
	{
		write(1, drawing + (i * zone->width), zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	FILE	*file;
	char	*drawing;
	t_zone	zone;

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	if (argc != 2)
		return (error_msg("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (error_msg("Error: Operation file corrupted\n"));
	if (!(get_zone(file, &zone)))
		return (clear_all(file, NULL) && error_msg("Error: Operation file corrupted\n"));
	if (!(drawing = malloc_drawing(&zone)))
		return (clear_all(file, NULL) && error_msg("Error: Malloc\n"));
	if (!(get_shapes(file, &zone, drawing)))
		return (clear_all(file, drawing) && error_msg("Error: Operation file corrupted\n"));
	draw_drawing(&zone, drawing);
	clear_all(file, drawing);
	return (0);
}
