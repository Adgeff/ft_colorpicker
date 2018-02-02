/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_color_picker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 15:38:03 by geargenc          #+#    #+#             */
/*   Updated: 2018/02/02 12:56:30 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <stdlib.h>
#include <stdio.h>

#define WIN_WIDTH 600
#define WIN_HEIGHT 400

#define GREY 0x00A2B5BF

typedef struct	s_img
{
	void		*ptr;
	char		*data;
	int			bpp;
	int			sl;
	int			endian;
	int			x_size;
	int			y_size;
	int			x;
	int			y;
}				t_img;

typedef struct	s_click
{
	int			button;
	int			x;
	int			y;
}				t_click;

typedef union		u_color
{
	int				color;
	unsigned char	byte[4];
}					t_color;

typedef struct	s_clrpick
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		bar;
	t_img		sqr;
	t_img		bar_cursor;
	t_img		sqr_cursor;
	int			(*f)(int, void *);
	void		*param;
	t_click		click;
}				t_clrpick;

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

void			img_draw_line(t_img *img, int y, int color)
{
	int			x;

	x = 0;
	while (x < img->x_size)
	{
		img_pixel_put(img, x, y, color);
		x++;
	}
}

/*void			ft_draw_bar(t_clrpick *clrpick)
{
	int			i;
	int			k;
	t_color		color;

	k = 3;
	color.color = 0;
	while (--k >= 0)
	{
		color.byte[k] = 0xFF;
		i = -1;
		while (++i <= (WIN_HEIGHT / 6))
		{
			color.byte[(k - 1 + 3) % 3] = 255000 / (WIN_HEIGHT / 6) * i / 1000;
			img_draw_line(&(clrpick->bar), i + ((2 - k) * (WIN_HEIGHT / 6) * 2),
				color.color);
		}
		color.byte[(k - 1 + 3) % 3] = 0xFF;
		while (--i > 0)
		{
			color.byte[k] = 255000 / (WIN_HEIGHT / 6) * i / 1000;
			img_draw_line(&(clrpick->bar), ((WIN_HEIGHT / 6) - i) + ((2 - k) *
				(WIN_HEIGHT / 6) * 2) + (WIN_HEIGHT / 6), color.color);
		}
		color.byte[k] = 0x00;
	}
}*/

int				ft_bar_red(int where, int size)
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

int				ft_bar_green(int where, int size)
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

int				ft_bar_blue(int where, int size)
{
	where = (where % size + size) % size;
	if (where < size / 6 * 2)
		return (0);
	if (where < size / 6 * 3)
		return ((255000 / (size / 6)) * (where % (size / 6)) / 1000);
	if (where < size / 6 * 5)
		return (255);
	return ((-255000 / (size / 6)) * (where % (size / 6)) / 1000 + 255);
}

void			ft_draw_bar(t_clrpick *clrpick)
{
	int			i;
	int			j;

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

int				ft_bar(t_clrpick *clrpick)
{
	clrpick->bar.x_size = WIN_WIDTH / 15;
	clrpick->bar.y_size = WIN_HEIGHT / 6 * 6;
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

void			ft_draw_bar_cursor(t_clrpick *clrpick)
{
	int	i;
	int	j;

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

int				ft_bar_cursor(t_clrpick *clrpick)
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

void			ft_draw_sqr_cursor(t_clrpick *clrpick)
{
	int			i;
	int			j;

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

int				ft_sqr_cursor(t_clrpick *clrpick)
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

int				ft_clrpick_release_event(int button, int x, int y, void *param)
{
	t_clrpick	*clrpick;

	clrpick = param;
	if (button == clrpick->click.button)
		clrpick->click.button = 0;
	return (0);
}

void			ft_close_color_picker(void *clrpick_ptr);

int				ft_clrpick_motion_event(int x, int y, void *param)
{
	t_clrpick	*clrpick;

	clrpick = param;
	if (clrpick->click.button == 1)
	{
		if (ft_isinimage(&(clrpick->bar), clrpick->click.x, clrpick->click.y))
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
		if (ft_isinimage(&(clrpick->sqr), clrpick->click.x, clrpick->click.y))
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
			clrpick->f(img_get_color(&(clrpick->sqr), x - clrpick->sqr.x, y - clrpick->sqr.y).color,
				clrpick->param);
		}
	}
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

void			ft_bzero(void *ptr, size_t len)
{
	size_t		i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)ptr)[i] = 0;
		i++;
	}
}

void			*ft_color_picker(void *mlx_ptr, int (*f)(int, void *),
				void *param)
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
		WIN_HEIGHT, "color_picker");
	ft_draw_window(clrpick);
	mlx_hook(clrpick->win_ptr, ButtonPress, ButtonPressMask,
		&ft_clrpick_click_event, clrpick);
	mlx_hook(clrpick->win_ptr, MotionNotify, PointerMotionMask,
		&ft_clrpick_motion_event, clrpick);
	mlx_hook(clrpick->win_ptr, ButtonRelease, ButtonReleaseMask,
		&ft_clrpick_release_event, clrpick);
	return (clrpick);
}

void			ft_close_color_picker(void *clrpick_ptr)
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

int				test(int color, void *param)
{
	t_clrpick	*clrpick;
	int			i;
	int			j;

	clrpick = param;
	j = 0;
	while (j < WIN_HEIGHT)
	{
		i = 0;
		while (i < WIN_WIDTH)
		{
			img_pixel_put(&(clrpick->bar), i, j, color);
			i++;
		}
		j++;
	}
	mlx_put_image_to_window(clrpick->mlx_ptr, clrpick->win_ptr, clrpick->bar.ptr, 0, 0);
	return (0);
}

int				main(void)
{
	t_clrpick	clrpick;
	void		*clrpick_ptr;

	clrpick.mlx_ptr = mlx_init();
	clrpick.win_ptr = mlx_new_window(clrpick.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "test");
	clrpick.bar.ptr = mlx_new_image(clrpick.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	clrpick.bar.data = mlx_get_data_addr(clrpick.bar.ptr, &(clrpick.bar.bpp), &(clrpick.bar.sl), &(clrpick.bar.endian));
	clrpick.bar.x_size = WIN_WIDTH;
	clrpick.bar.y_size = WIN_HEIGHT;
	clrpick_ptr = ft_color_picker(clrpick.mlx_ptr, &test, &clrpick);
	mlx_loop(clrpick.mlx_ptr);
	return (0);
}
