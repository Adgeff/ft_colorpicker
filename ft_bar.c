/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/06 16:00:14 by geargenc          #+#    #+#             */
/*   Updated: 2018/02/06 16:13:13 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_clrpick.h"

static int	ft_bar_red(int where, int size)
{
	where = (where % size + size) % size;
	if (where < size / 6 * 1)
		return (255);
	if (where < size / 6 * 2)
		return ((-255000 / (size / 6)) * (where % (size / 6)) / 1000 + 255);
	if (where < size / 6 * 4)
		return (0);
	if (where < size / 6 * 5)
		return ((255000 / (size / 6)) * (where % (size / 6)) / 1000);
	return (255);
}

static int	ft_bar_green(int where, int size)
{
	where = (where % size + size) % size;
	if (where < size / 6 * 1)
		return ((255000 / (size / 6)) * (where % (size / 6)) / 1000);
	if (where < size / 6 * 3)
		return (255);
	if (where < size / 6 * 4)
		return ((-255000 / (size / 6)) * (where % (size / 6)) / 1000 + 255);
	return (0);
}

static int	ft_bar_blue(int where, int size)
{
	where = (where % size + size) % size;
	if (where < size / 6 * 2)
		return (0);
	if (where < size / 6 * 3)
		return ((255000 / (size / 6)) * (where % (size / 6)) / 1000);
	if (where < size / 6 * 5)
		return (255);
	if (where < size / 6 * 6)
		return ((-255000 / (size / 6)) * (where % (size / 6)) / 1000 + 255);
	return (0);
}

static void	ft_draw_bar(t_clrpick *clrpick)
{
	int		i;
	int		j;

	j = 0;
	while (j < clrpick->bar.y_size)
	{
		i = 0;
		while (i < clrpick->bar.x_size)
		{
			img_pixel_put(&(clrpick->bar), i, j,
				((ft_bar_red(j, clrpick->bar.y_size) << 16) |
				(ft_bar_green(j, clrpick->bar.y_size) << 8) |
				(ft_bar_blue(j, clrpick->bar.y_size) << 0)) & 0x00FFFFFF);
			i++;
		}
		j++;
	}
}

int			ft_bar(t_clrpick *clrpick)
{
	clrpick->bar.x_size = WIN_WIDTH / 15;
	clrpick->bar.y_size = WIN_HEIGHT;
	clrpick->bar.x = WIN_WIDTH / 15 * 1;
	clrpick->bar.y = 0;
	if (!(clrpick->bar.ptr = mlx_new_image(clrpick->mlx_ptr,
		clrpick->bar.x_size, clrpick->bar.y_size)) ||
		!(clrpick->bar.data = mlx_get_data_addr(clrpick->bar.ptr,
		&(clrpick->bar.bpp), &(clrpick->bar.sl), &(clrpick->bar.endian))))
		return (1);
	ft_draw_bar(clrpick);
	return (0);
}
