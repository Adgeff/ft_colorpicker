/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_color_picker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 15:38:03 by geargenc          #+#    #+#             */
/*   Updated: 2018/01/29 18:06:21 by jhache           ###   ########.fr       */
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

	clrpick->bar_cursor.x_size = clrpick->bar.x_size / 2;
	clrpick->bar_cursor.y_size = (WIN_HEIGHT / 20) + 1;
	clrpick->bar_cursor.ptr = mlx_new_image(mlx_ptr, clrpick->bar_cursor.x_size,
											clrpick->bar_cursor.y_size);
	clrpick->bar_cursor.data =
		mlx_get_data_addr(clrpick->bar_cursor.ptr, &(clrpick->bar_cursor.bpp),
						&(clrpick->bar_cursor.sl), &(clrpick->bar_cursor.endian));
	clrpick->color.color = GREY;
	y = 0;
	while (y < clrpick->bar_cursor.y_size)
	{
		x = 0;
		while (x < clrpick->bar_cursor.x_size)
			img_pixel_put(&(clrpick->bar_cursor), x++, y, 0xFF000000);
		++y;
	}
/*	//	***** CURSEUR LATERAL, A DROITE *****
	x = 0;
	y = clrpick->bar_cursor.y_size / 2;
	while (x < 3)// (x_size - ?) pour compenser l'incrementation de x
	{
		ft_draw_segment(x, y, x + (clrpick->bar_cursor.x_size - 3), y + (clrpick->bar_cursor.y_size / 2), clrpick);
		ft_draw_segment(x, y, x + (clrpick->bar_cursor.x_size - 3), y - (clrpick->bar_cursor.y_size / 2), clrpick);
		++x;
	}*/
	//	***** CURSEUR LATERAL, A GAUCHE *****
	x = 0;
	y = clrpick->bar_cursor.y_size / 2;
	while (x < 3)//	(x_size - ?) pour compenser l'incrementation de x.
//					on peut remplacer la valeur fixe (?) par une proportion de la taille de la fenetre (WIN_WIDTH / 200 par exemple)
	{
		ft_draw_segment(x + (clrpick->bar_cursor.x_size - 3), y, x, y + (clrpick->bar_cursor.y_size / 2), clrpick);
		ft_draw_segment(x + (clrpick->bar_cursor.x_size - 3), y, x, y - (clrpick->bar_cursor.y_size / 2), clrpick);
		++x;
	}
	return (0);
}

int				m222lx_color_bar_cursor(void *mlx_ptr, t_clrpick *clrpick)//FLAT CURSOR
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
			img_pixel_put(&(clrpick->bar_cursor), j++, i, GREY);
		++i;
	}
//	***** CURSEUR VIDE ******
/*	while (i < clrpick->bar_cursor.y_size)
	{
		j = 0;
		img_pixel_put(&(clrpick->bar_cursor), 0, i, GREY);
		img_pixel_put(&(clrpick->bar_cursor),
					clrpick->bar_cursor.x_size - 1, i, GREY);
		while (j < clrpick->bar_cursor.x_size)
		{
			if (i == 0 || i == clrpick->bar_cursor.y_size - 1)
				img_pixel_put(&(clrpick->bar_cursor), j++, i, GREY);
			else
				img_pixel_put(&(clrpick->bar_cursor), j++, i, 0xFF000000);
		}
			++i;
	}*/
	return (0);
}

int				mlx_color_sqr(void *mlx_ptr, t_clrpick *clrpick, t_color color)
{
	int		tmp;
	int		k;
	int		x;
	int		y;
	t_color	pxl_clr;

	clrpick->sqr.x_size = WIN_WIDTH / 5 * 4;
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
				tmp = color.byte[k] * 1000;
				if (color.byte[k] != 255)
					tmp += (x * 1000) / (clrpick->sqr.x_size / (255 - color.byte[k]));
				if (color.byte[k] != 0)
				tmp -= (y * 1000) / (clrpick->sqr.y_size / color.byte[k]);
//				tmp /= (1000 - ((y * 1000) / clrpick->sqr.y_size));
//				tmp /= ((((y == 0) ? 1 : y) * 1000) / clrpick->sqr.y_size);
				pxl_clr.byte[k] = tmp / 1000;
			}
			pxl_clr.byte[3] = 0;
			img_pixel_put(&(clrpick->sqr), x, y, pxl_clr.color);
			++x;
		}
		++y;
	}
	return (0);
}

void			*mlx_color_picker(void *mlx_ptr, int (*f)(int, void *),
				void *params)
{
	t_clrpick	*clrpick;
	t_color		color_test;

	color_test.color = 0xFF0000;
	(void)f;
	(void)params;
	if (!(clrpick = (t_clrpick *)malloc(sizeof(t_clrpick))) ||
		mlx_color_bar(mlx_ptr, clrpick) ||
		mlx_color_sqr(mlx_ptr, clrpick, color_test) ||
		mlx_color_bar_cursor(mlx_ptr, clrpick))// ||
//		mlx_color_sqr_cursor(mlx_ptr, clrpick))
		return (NULL);
	clrpick->win_ptr = mlx_new_window(mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "test");
//	********** BAR **********
//	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar.ptr, BAR_POSX, BAR_POSY);//FLAT CURSOR, RIGHT CURSOR
	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar.ptr, BAR_POSX + WIN_WIDTH / 15, BAR_POSY);//LEFT CURSOR
//	********** BAR_CURSOR **********
	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar_cursor.ptr,
							BAR_POSX + WIN_WIDTH / 15 - (clrpick->bar_cursor.x_size), BAR_POSY + 30);//LEFT CURSOR
//	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar_cursor.ptr,
//							BAR_POSX + WIN_WIDTH / 15, BAR_POSY + 30);//RIGHT CURSOR
//	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar_cursor.ptr,
//							BAR_POSX, BAR_POSY + 30);//FLAT CURSOR
//	********** SQUARE **********
	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->sqr.ptr, SQR_POSX, SQR_POSY);
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
