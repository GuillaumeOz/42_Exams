/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gozsertt <gozsertt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 11:59:04 by gozsertt          #+#    #+#             */
/*   Updated: 2020/01/07 13:01:22 by gozsertt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_MODEL_H
# define GET_NEXT_LINE_MODEL_H

# define SENTINAL 1

#include <stdlib.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/uio.h>

int	get_next_line(char **line);

#endif