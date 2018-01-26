/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_color_picker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 15:38:03 by geargenc          #+#    #+#             */
/*   Updated: 2018/01/26 17:35:41 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <stdlib.h>
#include <stdio.h>

#define WIN_WIDTH 300
#define WIN_HEIGHT 200

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

typedef struct	s_clrpick
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		bar;
	t_img		sqr;
	t_img		bar_cursor;
	t_img		sqr_cursor;
	t_button	button;
}				t_clrpick;

typedef union		u_color
{
	int				color;
	unsigned char	byte[4];
}					t_color;

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

int				mlx_color_bar(void *mlx_ptr, t_clrpick *clrpick)
{
	int			stage_len;
	int			i;
	int			j;
	int			k;
	t_color		color;

	clrpick->bar.ptr = mlx_new_image(mlx_ptr, WIN_WIDTH / 15, WIN_HEIGHT);
	clrpick->bar.data = mlx_get_data_addr(clrpick->bar.ptr, &(clrpick->bar.bpp), &(clrpick->bar.sl), &(clrpick->bar.endian));
	clrpick->bar.x_size = WIN_WIDTH / 15;
	clrpick->bar.y_size = WIN_HEIGHT;
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

void			*mlx_color_picker(void *mlx_ptr, int (*f)(int, void *),
				void *params)
{
	t_clrpick	*clrpick;

	(void)f;
	(void)params;
	if (!(clrpick = (t_clrpick *)malloc(sizeof(t_clrpick))) ||
		mlx_color_bar(mlx_ptr, clrpick)) //||
//		mlx_color_bar_cursor(mlx_ptr, clrpick) ||
//		mlx_color_sqr_cursor(mlx_ptr, clrpick))
		return (NULL);
	clrpick->win_ptr = mlx_new_window(mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "test");
	mlx_put_image_to_window(mlx_ptr, clrpick->win_ptr, clrpick->bar.ptr, 0, 0);
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
