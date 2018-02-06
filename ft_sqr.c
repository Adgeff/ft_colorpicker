/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/06 16:20:19 by geargenc          #+#    #+#             */
/*   Updated: 2018/02/06 16:20:38 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_clrpick.h"

int				ft_draw_sqr(t_clrpick *clrpick, t_color color)
{
	int			k;
	int			x;
	int			y;
	t_color		pxl_clr;

	y = -1;
	while (++y < clrpick->sqr.y_size)
	{
		x = -1;
		while (++x < clrpick->sqr.x_size)
		{
			k = -1;
			while (++k < 3)
			{
				pxl_clr.byte[k] = color.byte[k] + ((255 - color.byte[k]) * 1000
					/ clrpick->sqr.x_size) * x / 1000;
				pxl_clr.byte[k] = pxl_clr.byte[k] - (pxl_clr.byte[k] * 1000 /
					clrpick->sqr.y_size) * y / 1000;
			}
			pxl_clr.byte[3] = 0;
			img_pixel_put(&(clrpick->sqr), x, y, pxl_clr.color);
		}
	}
	return (0);
}

int				ft_sqr(t_clrpick *clrpick)
{
	clrpick->sqr.x_size = WIN_WIDTH / 3 * 2;
	clrpick->sqr.y_size = WIN_HEIGHT;
	clrpick->sqr.x = WIN_WIDTH / 5;
	clrpick->sqr.y = 0;
	if (!(clrpick->sqr.ptr = mlx_new_image(clrpick->mlx_ptr,
		clrpick->sqr.x_size, clrpick->sqr.y_size)) ||
		!(clrpick->sqr.data = mlx_get_data_addr(clrpick->sqr.ptr,
		&(clrpick->sqr.bpp), &(clrpick->sqr.sl), &(clrpick->sqr.endian))))
		return (1);
	ft_draw_sqr(clrpick, (t_color)0xFF0000);
	return (0);
}
