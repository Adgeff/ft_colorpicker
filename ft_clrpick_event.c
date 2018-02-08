/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clrpick_event.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/06 16:25:58 by geargenc          #+#    #+#             */
/*   Updated: 2018/02/08 12:56:40 by jhache           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_clrpick.h"

int				ft_clrpick_release_event(int button, int x, int y, void *param)
{
	t_clrpick	*clrpick;

	clrpick = param;
	if (button == clrpick->click.button)
		clrpick->click.button = 0;
	x = 0;
	y = 0;
	return (0);
}

int				ft_clrpick_click_event(int button, int x, int y, void *param)
{
	t_clrpick	*clrpick;

	clrpick = param;
	if (clrpick->click.button == 0)
	{
		clrpick->click.button = button;
		clrpick->click.x = x;
		clrpick->click.y = y;
		ft_clrpick_motion_event(x, y, param);
	}
	return (0);
}

static void		ft_clrpick_motion_bar(int y, t_clrpick *clrpick)
{
	if (y < clrpick->bar.y)
		y = clrpick->bar.y;
	else if (y >= clrpick->bar.y + clrpick->bar.y_size)
		y = clrpick->bar.y + clrpick->bar.y_size - 1;
	clrpick->bar_cursor.y = y - clrpick->bar_cursor.y_size / 2;
	ft_draw_sqr(clrpick, img_get_color(&(clrpick->bar), 0, y));
	ft_draw_window(clrpick);
	clrpick->f(img_get_color(&(clrpick->sqr), clrpick->sqr_cursor.x +
				clrpick->sqr_cursor.x_size / 2 - clrpick->sqr.x,
				clrpick->sqr_cursor.y + clrpick->sqr_cursor.y_size / 2 -
				clrpick->sqr.y).color,
			clrpick->param);
}

static void		ft_clrpick_motion_sqr(int x, int y, t_clrpick *clrpick)
{
	if (x < clrpick->sqr.x)
		x = clrpick->sqr.x;
	else if (x >= clrpick->sqr.x + clrpick->sqr.x_size)
		x = clrpick->sqr.x + clrpick->sqr.x_size - 1;
	if (y < clrpick->sqr.y)
		y = clrpick->sqr.y;
	else if (y >= clrpick->sqr.y + clrpick->sqr.y_size)
		y = clrpick->sqr.y + clrpick->sqr.y_size - 1;
	clrpick->sqr_cursor.x = x - clrpick->sqr_cursor.x_size / 2;
	clrpick->sqr_cursor.y = y - clrpick->sqr_cursor.y_size / 2;
	ft_draw_window(clrpick);
	clrpick->f(img_get_color(&(clrpick->sqr), x - clrpick->sqr.x, y -
				clrpick->sqr.y).color, clrpick->param);
}

int				ft_clrpick_motion_event(int x, int y, void *param)
{
	t_clrpick	*clrpick;

	clrpick = param;
	if (clrpick->click.button == 1)
	{
		if (ft_isinimage(&(clrpick->bar), clrpick->click.x, clrpick->click.y))
			ft_clrpick_motion_bar(y, clrpick);
		if (ft_isinimage(&(clrpick->sqr), clrpick->click.x, clrpick->click.y))
			ft_clrpick_motion_sqr(x, y, clrpick);
	}
	return (0);
}
