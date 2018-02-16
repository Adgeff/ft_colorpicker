/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_colorpicker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 15:38:03 by geargenc          #+#    #+#             */
/*   Updated: 2018/02/16 23:22:47 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_clrpick.h"

void			ft_draw_window(t_clrpick *clrpick)
{
	mlx_clear_window(clrpick->mlx_ptr, clrpick->win_ptr);
	mlx_put_image_to_window(clrpick->mlx_ptr, clrpick->win_ptr,
		clrpick->bar.ptr, clrpick->bar.x, clrpick->bar.y);
	mlx_put_image_to_window(clrpick->mlx_ptr, clrpick->win_ptr,
		clrpick->sqr.ptr, clrpick->sqr.x, clrpick->sqr.y);
	mlx_put_image_to_window(clrpick->mlx_ptr, clrpick->win_ptr,
		clrpick->bar_cursor.ptr, clrpick->bar_cursor.x, clrpick->bar_cursor.y);
	mlx_put_image_to_window(clrpick->mlx_ptr, clrpick->win_ptr,
		clrpick->sqr_cursor.ptr, clrpick->sqr_cursor.x, clrpick->sqr_cursor.y);
}

static void		ft_bzero(void *ptr, size_t len)
{
	size_t		i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)ptr)[i] = 0;
		i++;
	}
}

void			*ft_colorpicker(void *mlx_ptr, int (*f)(int, void *),
				void *param, char *name)
{
	t_clrpick	*clrpick;

	if (!(clrpick = (t_clrpick *)malloc(sizeof(t_clrpick))))
		return (NULL);
	ft_bzero(clrpick, sizeof(t_clrpick));
	clrpick->mlx_ptr = mlx_ptr;
	clrpick->f = f;
	clrpick->param = param;
	if (ft_bar(clrpick) ||
		ft_bar_cursor(clrpick) ||
		ft_sqr(clrpick) ||
		ft_sqr_cursor(clrpick))
		return (NULL);
	clrpick->win_ptr = mlx_new_window(mlx_ptr, WIN_WIDTH,
		WIN_HEIGHT, name);
	ft_draw_window(clrpick);
	mlx_hook(clrpick->win_ptr, BUTTONPRESS, BUTTONPRESSMASK,
		&ft_clrpick_click_event, clrpick);
	mlx_hook(clrpick->win_ptr, MOTIONNOTIFY, POINTERMOTIONMASK,
		&ft_clrpick_motion_event, clrpick);
	mlx_hook(clrpick->win_ptr, BUTTONRELEASE, BUTTONRELEASEMASK,
		&ft_clrpick_release_event, clrpick);
	return (clrpick);
}

void			ft_close_colorpicker(void *clrpick_ptr)
{
	t_clrpick	*clrpick;

	clrpick = clrpick_ptr;
	mlx_destroy_image(clrpick->mlx_ptr, clrpick->bar.ptr);
	mlx_destroy_image(clrpick->mlx_ptr, clrpick->sqr.ptr);
	mlx_destroy_image(clrpick->mlx_ptr, clrpick->bar_cursor.ptr);
	mlx_destroy_image(clrpick->mlx_ptr, clrpick->sqr_cursor.ptr);
	mlx_destroy_window(clrpick->mlx_ptr, clrpick->win_ptr);
	free(clrpick);
}

void			ft_reset_colorpicker(void *clrpick_ptr)
{
	t_clrpick	*clrpick;
	t_color		color;

	clrpick = clrpick_ptr;
	clrpick->bar_cursor.x = clrpick->bar.x - ((clrpick->bar_cursor.x_size -
		clrpick->bar.x_size) / 2);
	clrpick->bar_cursor.y = -(clrpick->bar_cursor.y_size / 2);
	clrpick->sqr_cursor.x = clrpick->sqr.x - clrpick->sqr_cursor.x_size / 2;
	clrpick->sqr_cursor.y = clrpick->sqr.y - clrpick->sqr_cursor.y_size / 2;
	color.color = 0xFF0000;
	ft_draw_sqr(clrpick, color);
	ft_draw_window(clrpick);
}
