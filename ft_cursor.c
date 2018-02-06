/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cursor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/06 16:03:43 by geargenc          #+#    #+#             */
/*   Updated: 2018/02/06 16:13:23 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_clrpick.h"

static void	ft_draw_bar_cursor(t_clrpick *clrpick)
{
	int		i;
	int		j;

	i = 0;
	j = -1;
	while (++j < clrpick->bar_cursor.y_size)
	{
		i = -1;
		if (j < 2 || j >= clrpick->bar_cursor.y_size - 2)
			while (++i < clrpick->bar_cursor.x_size)
				img_pixel_put(&(clrpick->bar_cursor), i, j, GREY);
		else
			while (++i < clrpick->bar_cursor.x_size)
				img_pixel_put(&(clrpick->bar_cursor), i, j, ((i < 2 ||
					i >= clrpick->bar_cursor.x_size - 2) ? GREY : 0xFF000000));
	}
}

int			ft_bar_cursor(t_clrpick *clrpick)
{
	clrpick->bar_cursor.x_size = clrpick->bar.x_size + 4;
	clrpick->bar_cursor.y_size = 7;
	clrpick->bar_cursor.x = clrpick->bar.x - ((clrpick->bar_cursor.x_size -
		clrpick->bar.x_size) / 2);
	clrpick->bar_cursor.y = -(clrpick->bar_cursor.y_size / 2);
	if (!(clrpick->bar_cursor.ptr = mlx_new_image(clrpick->mlx_ptr,
		clrpick->bar_cursor.x_size, clrpick->bar_cursor.y_size)) ||
	!(clrpick->bar_cursor.data = mlx_get_data_addr(clrpick->bar_cursor.ptr,
		&(clrpick->bar_cursor.bpp), &(clrpick->bar_cursor.sl),
		&(clrpick->bar_cursor.endian))))
		return (1);
	ft_draw_bar_cursor(clrpick);
	return (0);
}

static void	ft_draw_sqr_cursor(t_clrpick *clrpick)
{
	int		i;
	int		j;

	j = 0;
	while (j < clrpick->sqr_cursor.y_size)
	{
		i = 0;
		while (i < clrpick->sqr_cursor.x_size)
		{
			if ((i >= clrpick->sqr_cursor.x_size / 2 - 1 &&
				i <= clrpick->sqr_cursor.x_size / 2 + 1) ||
				(j >= clrpick->sqr_cursor.y_size / 2 - 1 &&
				j <= clrpick->sqr_cursor.y_size / 2 + 1))
				img_pixel_put(&(clrpick->sqr_cursor), i, j, 0x42000000 | GREY);
			else
				img_pixel_put(&(clrpick->sqr_cursor), i, j, 0xFF000000);
			i++;
		}
		j++;
	}
}

int			ft_sqr_cursor(t_clrpick *clrpick)
{
	clrpick->sqr_cursor.x_size = 15;
	clrpick->sqr_cursor.y_size = clrpick->sqr_cursor.x_size;
	clrpick->sqr_cursor.x = clrpick->sqr.x - clrpick->sqr_cursor.x_size / 2;
	clrpick->sqr_cursor.y = clrpick->sqr.y - clrpick->sqr_cursor.y_size / 2;
	if (!(clrpick->sqr_cursor.ptr = mlx_new_image(clrpick->mlx_ptr,
		clrpick->sqr_cursor.x_size, clrpick->sqr_cursor.y_size)) ||
		!(clrpick->sqr_cursor.data = mlx_get_data_addr(clrpick->sqr_cursor.ptr,
		&(clrpick->sqr_cursor.bpp), &(clrpick->sqr_cursor.sl),
		&(clrpick->sqr_cursor.endian))))
		return (1);
	ft_draw_sqr_cursor(clrpick);
	return (0);
}
