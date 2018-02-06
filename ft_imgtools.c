/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_imgtools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/06 15:59:10 by geargenc          #+#    #+#             */
/*   Updated: 2018/02/06 16:13:35 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_clrpick.h"

void			img_pixel_put(t_img *img, int x, int y, int color)
{
	int			*i;

	if (x >= 0 && x < img->x_size && y >= 0 && y < img->y_size)
	{
		i = (int *)img->data;
		i = i + y * img->x_size + x;
		*i = color;
	}
}

t_color			img_get_color(t_img *img, int x, int y)
{
	t_color		*color;

	color = (t_color *)img->data;
	if (x >= 0 && x < img->x_size && y >= 0 && y < img->y_size)
		return (color[y * img->x_size + x]);
	return ((t_color)0);
}

int				ft_isinimage(t_img *img, int x, int y)
{
	return (x >= img->x && x < img->x + img->x_size &&
		y >= img->y && y < img->y + img->y_size);
}
