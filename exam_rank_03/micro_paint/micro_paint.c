/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gozsertt <gozsertt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/03 12:01:22 by gozsertt          #+#    #+#             */
/*   Updated: 2020/09/02 12:57:25 by gozsertt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct	s_zone
{
	char	background;
	int		width;
	int		height;
}				t_zone;

typedef struct	s_shape
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	color;
}				t_shape;

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return(i);
}

int clear_all(FILE *file, char *drawing)
{
	fclose(file);
	if (drawing)
		free(drawing);
	return(1);
}

int error_msg(char *msg, int ret)
{
	write(1, msg, ft_strlen(msg));
	return(ret);
}

int get_zone(FILE *file, t_zone *zone)
{
	int scanf_ret;

	if ((scanf_ret = fscanf(file, "%d %d %c\n",&zone->width, &zone->height, &zone->background)) != 3)
		return (0);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
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
	while (i < zone->width * zone->height)
		drawing[i++] = zone->background;
	return (drawing);
}

//Si 1 point est définis par (Xa, Ya)
// Et qu'un rectangle est defini par son coin en haut a gauche (Xtl, Ytl) et son coin en bas a droite (Xbr, Ybr)
// If Xtl <= Xa <= Xbr and Ytl <= Ya <= Ybr alors le point est dans le rectangle

void empty_rectangle(char *drawing, t_zone *zone, t_shape *shape)
{
	int y;
	int x;

	y = 0;
	while (y < zone->height && shape->y + y < zone->height)
	{
		x = 0;
		while (x < zone->width && shape->x + x < zone->width)
		{
			if (x == 0 || y == 0 || x == shape->width - 1 || y == shape->height - 1)
				drawing[(y * zone->height) + x] = shape->color;
			x++;
		}
		y++;
	}
}

//Si 1 point est définis par (Xa, Ya)
// Et qu'un rectangle est defini par son coin en haut a gauche (Xtl, Ytl) et son coin en bas a droite (Xbr, Ybr)
// If Xtl <= Xa <= Xbr and Ytl <= Ya <= Ybr alors le point est dans le rectangle


void full_rectangle(char *drawing, t_zone *zone, t_shape *shape)
{
	int y;
	int x;

	y = 0;
	while (y < shape->height && shape->y + y < zone->height)
	{
		x = 0;
		while (x < shape->width && shape->x + x < zone->width)
		{
			drawing[(y * zone->height) + x] = shape->color;
			x++;
		}
		y++;
	}
}

int get_shapes(FILE *file, t_zone *zone, char *drawing)
{
	int	scanf_ret;
	t_shape	tmp;

	while ((scanf_ret = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.width, &tmp.height, &tmp.color)) == 6)
	{
		if (tmp.width <= 0.00000000 || tmp.height <= 0.00000000 || (tmp.type != 'r' && tmp.type != 'R'))
			return (0);
		if (tmp.type == 'R')
			full_rectangle(drawing, zone, &tmp);
		if (tmp.type == 'r')
			empty_rectangle(drawing, zone, &tmp);
	}
	if (scanf_ret != -1)
		return (0);
	return (1);
}

void draw_drawing(char *drawing, t_zone *zone)
{
	int i;

	i = 0;
	while ( i < zone->height)
	{
		write(1, drawing + (i * zone->width), zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_zone	zone;
	FILE	*file;
	char	*drawing;

	zone.background = 0;
	zone.width = 0;
	zone.height = 0;
	if (argc != 2)
		return(error_msg("Error: argument\n", 1));
	if (!(file = fopen(argv[1], "r")))
		return(error_msg("Error: Operation file corrupted\n", 1));
	if (!(get_zone(file, &zone)))
		return(clear_all(file, NULL) && error_msg("Error: Operation file corrupted\n", 1));
	if (!(drawing = malloc_drawing(&zone)))
		return(clear_all(file, NULL) && error_msg("Error: Malloc\n", 1));
	if (!(get_shapes(file, &zone, drawing)))
		return(clear_all(file, drawing) && error_msg("Error: Operation file corrupted\n", 1));
	draw_drawing(drawing, &zone);
	clear_all(file, drawing);
	return(0);
}
