/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_color_picker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 15:38:03 by geargenc          #+#    #+#             */
/*   Updated: 2018/01/26 19:49:54 by jhache           ###   ########.fr       */
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

void	ft_swap_couple(int *x1, int *y1, int *x2, int *y2)
{
	int	tmp;

	if (*x1 > *x2)
	{
		tmp = *x2;
		*x2 = *x1;
		*x1 = tmp;
		tmp = *y2;
		*y2 = *y1;
		*y1 = tmp;
	}
}

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

	ft_swap_couple(&x1, &y1, &x2, &y2);
	err.ye = y2 - y1;
	err.xe = x2 - x1;
	err.dx = err.xe * 2;
	err.dy = err.ye * 2;
	while ((x1 < x2 || ((err.dy > 0) ? y1 < y2 : y1 > y2)))
	{
		img_pixel_put(&(clrpick->bar_cursor), x1, y1, clrpick->color.color);
		err.ye -= ABS(err.dx);
		err.xe -= ABS(err.dy);
		if (err.ye <= 0)
		{
			++x1;
			err.ye += ABS(err.dy);
		}
		if (err.xe < 0)
		{
			((err.dy < 0) ? --y1 : ++y1);
			err.xe += ABS(err.dx);
		}
	}
	img_pixel_put(&(clrpick->bar_cursor), x1, y1, clrpick->color.color);//seulement si curseur a gauche
}

int				mlx_color_bar(void *mlx_ptr, t_clrpick *clrpick)
{
	int			stage_len;
	int			i;
	int			j;
	int			k;
	t_color		color;

	clrpick->bar.x_size = WIN_WIDTH / 15;
	clrpick->bar.y_size = WIN_HEIGHT;
	clrpick->bar.ptr = mlx_new_image(mlx_ptr, clrpick->bar.x_size, clrpick->bar.y_size);
	clrpick->bar.data = mlx_get_data_addr(clrpick->bar.ptr, &(clrpick->bar.bpp),
										&(clrpick->bar.sl), &(clrpick->bar.endian));
	stage_len = WIN_HEIGHT / 6;
	k = 3;
	color.color = 0;
	while (--k >= 0)
	{
		color.byte[k] = 0xFF;
		i = -1;
		while (++i <= stage_len)
		{
			color.byte[(k - 1 + 3) % 3] = 255000 / stage_len * i / 1000;
			j = -1;
			while (++j < WIN_WIDTH / 15)
				img_pixel_put(&(clrpick->bar), j, i + ((2 - k) * stage_len * 2), color.color);
		}
		color.byte[(k - 1 + 3) % 3] = 0xFF;
		while (--i > 0)
		{
			color.byte[k] = 255000 / stage_len * i / 1000;
			j = -1;
			while (++j < WIN_WIDTH / 15)
				img_pixel_put(&(clrpick->bar), j, (stage_len - i) + ((2 - k) * stage_len * 2) + stage_len, color.color);
		}
		color.byte[k] = 0x00;
	}
	return (0);
}

int				mlx_color_bar_cursor(void *mlx_ptr, t_clrpick *clrpick)//ARROW CURSOR
{
	int	x;
	int	y;

	clrpick->bar_cursor.x_size = (WIN_WIDTH / 40) + 5;
	clrpick->bar_cursor.y_size = (WIN_HEIGHT / 20) + 1;
	clrpick->bar_cursor.ptr = mlx_new_image(mlx_ptr, clrpick->bar_cursor.x_size,
											clrpick->bar_cursor.y_size);
	clrpick->bar_cursor.data =
		mlx_get_data_addr(clrpick->bar_cursor.ptr, &(clrpick->bar_cursor.bpp),
						&(clrpick->bar_cursor.sl), &(clrpick->bar_cursor.endian));
	clrpick->color.color = GREY;
	//	***** CURSEUR LATERAL, A DROITE *****
	x = 0;
	y = 10;
	while (x < 5)
	{
		ft_draw_segment(x, y, x + 15, y + 10, clrpick);
		ft_draw_segment(x, y, x + 15, y - 10, clrpick);
		++x;
	}
	//	***** CURSEUR LATERAL, A GAUCHE *****
/*	x = 0;
	y = 10;
	while (x < 3)
	{
		ft_draw_segment(x + 15, y, x, y + 10, clrpick);
		ft_draw_segment(x + 15, y, x, y - 10, clrpick);
		++x;
	}*/
	return (0);
}

int				m22lx_color_bar_cursor(void *mlx_ptr, t_clrpick *clrpick)//FLAT CURSOR
{
	int	i;
	int	j;

	i = 0;
	clrpick->bar_cursor.x_size = WIN_WIDTH / 15;
	clrpick->bar_cursor.y_size = 3;
	clrpick->bar_cursor.ptr = mlx_new_image(mlx_ptr, clrpick->bar_cursor.x_size,
											clrpick->bar_cursor.y_size);
	clrpick->bar_cursor.data =
		mlx_get_data_addr(clrpick->bar_cursor.ptr, &(clrpick->bar_cursor.bpp),
						&(clrpick->bar_cursor.sl), &(clrpick->bar_cursor.endian));
	clrpick->color.color = GREY;
//	***** CURSEUR PLEIN *****
	while (i < clrpick->bar_cursor.y_size)
	{
		j = 0;
		while (j < clrpick->bar_cursor.x_size)
			img_pixel_put(&(clrpick->bar_cursor), j++, i, 0x00A2B5BF);
		++i;
	}
//	***** CURSEUR VIDE ******
/*	while (i < clrpick->bar_cursor.y_size)
	{
		j = 0;
		img_pixel_put(&(clrpick->bar_cursor), 0, i, 0x00A2B5BF);
		img_pixel_put(&(clrpick->bar_cursor),
					clrpick->bar_cursor.x_size - 1, i, 0x00A2B5BF);
		while (j < clrpick->bar_cursor.x_size)
		{
			if (i == 0 || i == clrpick->bar_cursor.y_size - 1)
				img_pixel_put(&(clrpick->bar_cursor), j++, i, 0x00A2B5BF);
			else
				img_pixel_put(&(clrpick->bar_cursor), j++, i, 0xFF000000);
		}
			++i;
	}*/
	return (0);
}

void			*mlx_color_picker(void *mlx_ptr, int (*f)(int, void *),
				void *params)
{
	t_clrpick	*clrpick;

	(void)f;
	(void)params;
	if (!(clrpick = (t_clrpick *)malloc(sizeof(t_clrpick))) ||
		mlx_color_bar(mlx_ptr, clrpick) ||
		mlx_color_bar_cursor(mlx_ptr, clrpick))// ||
//		mlx_color_sqr_cursor(mlx_ptr, clrpick))
		return (NULL);
	clrpick->win_ptr = mlx_new_window(mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "test");
	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar.ptr, BAR_POSX, BAR_POSY);//FLAT CURSOR, RIGHT CURSOR
//	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar.ptr, BAR_POSX + WIN_WIDTH / 15, BAR_POSY);//LEFT CURSOR
//	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar_cursor.ptr,
//							BAR_POSX + WIN_WIDTH / 30, BAR_POSY + 30);//LEFT CURSOR
	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar_cursor.ptr,
							BAR_POSX + WIN_WIDTH / 15, BAR_POSY + 30);//RIGHT CURSOR
//	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar_cursor.ptr,
//							BAR_POSX, BAR_POSY + 30);//FLAT CURSOR
	clrpick->button = none;
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
