/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_color_picker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 15:38:03 by geargenc          #+#    #+#             */
/*   Updated: 2018/01/29 21:59:57 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <stdlib.h>
#include <stdio.h>

#define ABS(x) (((x) < 0) ? ((x) * -1) : (x))

#define WIN_WIDTH 600
#define WIN_HEIGHT 400

#define BAR_POSX 0
#define BAR_POSY 0

#define SQR_POSX WIN_WIDTH / 5
#define SQR_POSY 0

#define GREY 0x00A2B5BF

typedef struct	s_bresenham
{
	int	dx;
	int	dy;
	int	xe;
	int	ye;
}				t_bresenham;

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

typedef enum	e_button
{
	none = 0,
	left = 1,
	right = 2,
	wheel = 4,
	down = 8,
	up = 16
}				t_button;

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
	t_button	button;
	t_color		color;
}				t_clrpick;

void		img_pixel_put(t_img *img, int x, int y, int color)
{
	int		*i;

	if (x >= 0 && x < img->x_size && y >= 0 && y < img->y_size)
	{
		i = (int *)img->data;
		i = i + y * img->x_size + x;
		*i = color;
	}
	else
		printf("Depassement image\n");
}

void	ft_draw_segment(int x1, int y1, int x2, int y2, t_clrpick *clrpick)
{
	t_bresenham	err;

	err.ye = y2 - y1;
	err.xe = x2 - x1;
	err.dx = err.xe * 2;
	err.dy = err.ye * 2;
	while (((err.dx > 0) ? x1 < x2 : x2 < x1)
		|| ((err.dy > 0) ? y1 < y2 : y1 > y2))
	{
		img_pixel_put(&(clrpick->bar_cursor), x1, y1, clrpick->color.color);
		err.ye -= ABS(err.dx);
		err.xe -= ABS(err.dy);
		if (err.ye <= 0)
		{
			((err.dx < 0) ? --x1 : ++x1);
			err.ye += ABS(err.dy);
		}
		if (err.xe < 0)
		{
			((err.dy < 0) ? --y1 : ++y1);
			err.xe += ABS(err.dx);
		}
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

void			ft_draw_bar(t_clrpick *clrpick)
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
}

int				ft_color_bar(t_clrpick *clrpick)
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

int				mlx_color_bar_cursor(void *mlx_ptr, t_clrpick *clrpick)
{
	int	i;
	int	j;

	i = 0;
	clrpick->bar_cursor.x_size = clrpick->bar.x_size + 4;
	clrpick->bar_cursor.y_size = 7;
	clrpick->bar_cursor.x = clrpick->bar.x - ((clrpick->bar_cursor.x_size - clrpick->bar.x_size) / 2);
	clrpick->bar_cursor.y = 0;
	clrpick->bar_cursor.ptr = mlx_new_image(mlx_ptr, clrpick->bar_cursor.x_size,
											clrpick->bar_cursor.y_size);
	clrpick->bar_cursor.data =
		mlx_get_data_addr(clrpick->bar_cursor.ptr, &(clrpick->bar_cursor.bpp),
						&(clrpick->bar_cursor.sl), &(clrpick->bar_cursor.endian));
	j = -1;
	while (++j < clrpick->bar_cursor.y_size)
	{
		i = -1;
		if (j < 2 || j >= clrpick->bar_cursor.y_size - 2)
			while (++i < clrpick->bar_cursor.x_size)
				img_pixel_put(&(clrpick->bar_cursor), i, j, GREY);
		else
			while (++i < clrpick->bar_cursor.x_size)
				img_pixel_put(&(clrpick->bar_cursor), i, j, ((i < 2 || i >= clrpick->bar_cursor.x_size - 2) ? GREY : 0xFF000000));
	}
	return (0);
}

int				mlx_color_sqr(void *mlx_ptr, t_clrpick *clrpick, t_color color)
{
	int		k;
	int		x;
	int		y;
	t_color	pxl_clr;

	clrpick->sqr.x_size = WIN_WIDTH / 3 * 2;
	clrpick->sqr.y_size = WIN_HEIGHT;
	clrpick->sqr.ptr = mlx_new_image(mlx_ptr, clrpick->sqr.x_size, clrpick->sqr.y_size);
	clrpick->sqr.data = mlx_get_data_addr(clrpick->sqr.ptr, &(clrpick->sqr.bpp),
										&(clrpick->sqr.sl), &(clrpick->sqr.endian));
	y = 0;
	while (y < clrpick->sqr.y_size)
	{
		x = 0;
		while (x < clrpick->sqr.x_size)
		{
			k = -1;
			while (++k < 3)
			{
				pxl_clr.byte[k] = color.byte[k] + ((255 - color.byte[k]) * 1000 / clrpick->sqr.x_size) * x / 1000;
				pxl_clr.byte[k] = pxl_clr.byte[k] - (pxl_clr.byte[k] * 1000 / clrpick->sqr.y_size) * y / 1000;
			}
			pxl_clr.byte[3] = 0;
			img_pixel_put(&(clrpick->sqr), x, y, pxl_clr.color);
			++x;
		}
		++y;
	}
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

int				clrpick_click_event(int button, int x, int y, void *params)
{
	t_clrpick	*clrpick;

	clrpick = params;
	printf("%d %d\n", x, y);
	if (x >= clrpick->bar.x && x < clrpick->bar.x + clrpick->bar.x_size &&
		y >= clrpick->bar.y && y < clrpick->bar.y + clrpick->bar.y_size)
	{
		clrpick->bar_cursor.y = y - clrpick->bar_cursor.y_size / 2;
		mlx_put_image_to_window(clrpick->mlx_ptr, clrpick->win_ptr,
			clrpick->bar_cursor.ptr, clrpick->bar_cursor.x,
			clrpick->bar_cursor.y);
		mlx_color_sqr(clrpick->mlx_ptr, clrpick, img_get_color(&(clrpick->bar), 0, y));
		mlx_put_image_to_window(clrpick->mlx_ptr, clrpick->win_ptr, clrpick->sqr.ptr, SQR_POSX, SQR_POSY);
	}
	return (0);
}

void			*mlx_color_picker(void *mlx_ptr, int (*f)(int, void *),
				void *params)
{
	t_clrpick	*clrpick;

	(void)f;
	(void)params;
	if (!(clrpick = (t_clrpick *)malloc(sizeof(t_clrpick))))
		return (NULL);
	clrpick->mlx_ptr = mlx_ptr;
	if (ft_color_bar(clrpick) ||
		mlx_color_bar_cursor(mlx_ptr, clrpick))// ||
//		mlx_color_sqr_cursor(mlx_ptr, clrpick))
		return (NULL);
	clrpick->win_ptr = mlx_new_window(mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "test");
	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar.ptr, clrpick->bar.x, clrpick->bar.y);
	clrpick->button = none;
	mlx_mouse_hook(clrpick->win_ptr, &clrpick_click_event, clrpick);
	mlx_loop(mlx_ptr);
	return (clrpick);
}

int				main(void)
{
	void		*mlx_ptr;

	mlx_ptr = mlx_init();
	mlx_color_picker(mlx_ptr, NULL, NULL);
	return (0);
}
