/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gozsertt <gozsertt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/03 12:01:22 by gozsertt          #+#    #+#             */
/*   Updated: 2020/08/05 16:57:08 by gozsertt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct	s_zone
{
	int		width;
	int		height;
	char	background;
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

int	check_zone(t_zone *zone)
{
	return (zone->width > 0 && zone->width <= 300
			&& zone->height > 0 && zone->height <= 300);
}

int get_zone(FILE *file, t_zone *zone)
{
	int scanf_ret;

	if ((scanf_ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3 )
		return (0);
	if ((check_zone(zone)) == 0)
		return (0);
	if (scanf_ret == -1)
		return (0);
	return (1);
}

char *malloc_drawing(t_zone *zone)
{
	char	*drawing;
	int		i;
	
	if (!(drawing = (char*)malloc(sizeof(char) * (zone->height * zone->width))))
		return (NULL);
	i = 0;
	while (i < zone->width * zone->height)
		drawing[i++] = zone->background;
	return (drawing);
}

int		in_rectangle(float x, float y, t_shape *shape)
{
	if (((x < shape->x || (shape->x + shape->width < x)) 
		|| (y < shape->y)) || (shape->y + shape->height < y))
		return (0);
	if (((x - shape->x < 1.00000000) || ((shape->x + shape->width) - x < 1.00000000))
		|| ((y - shape->y < 1.00000000 || ((shape->y + shape->height) - y < 1.00000000))))
		return (2);
	return (1);
}

int	check_shape(t_shape *shape)
{
	return (shape->width > 0.00000000 && shape->height > 0.00000000
			&& (shape->type == 'r' || shape->type == 'R'));
}

void fill_drawing(char **drawing, t_zone *zone, t_shape *shape)
{
	int i;
	int j;
	int ret;

	i = 0;
	while (i < zone->height)
	{
		j = 0;
		while (j < zone->width)
		{
			ret = in_rectangle(j, i, shape);
			if ((shape->type == 'r' && ret == 2)
				|| (shape->type == 'R' && ret == 1))
				(*drawing)[(int)((i * zone->width) + j)] = shape->color;
			j++;
		}
		i++;
	}
}

int	get_shapes(FILE *file, t_zone *zone, char **drawing)
{
	int		scanf_ret;
	t_shape	tmp;

	while ((scanf_ret = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.width, &tmp.height, &tmp.color)) == 6)
	{
		if ((check_shape(&tmp)) == 0)
			return (0);
		fill_drawing(drawing, zone, &tmp);
	}
	if (scanf_ret != -1)
		return (0);
	return (1);
}

void draw_drawing(char *drawing, t_zone *zone)
{
	int		i;

	i = 0;
	while (i < zone->height)
	{
		write(1, drawing + (i * zone->height), zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_zone	zone;
	FILE	*file;
	char	*drawing;

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	drawing = NULL;
	if (argc != 2)
		return (error_msg("Error: argument\n", 1));
	if (!(file = fopen(argv[1], "r")))
		return (error_msg("Error: Operation file corrupted\n", 1));
	if (!(get_zone(file, &zone)))
		return (clear_all(file, NULL) && error_msg("Error: Operation file corrupted\n", 1));
	if (!(drawing = malloc_drawing(&zone)))
		return (clear_all(file, NULL) && error_msg("Error: Malloc\n", 1));
	if (!(get_shapes(file, &zone, &drawing)))
		return (clear_all(file, drawing) && error_msg("Error: Operation file corrupted\n", 1));
	draw_drawing(drawing, &zone);
 	clear_all(file, drawing);
	return (0);
}
