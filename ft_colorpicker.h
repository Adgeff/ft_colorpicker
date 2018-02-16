/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_colorpicker.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geargenc <geargenc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/06 16:14:09 by geargenc          #+#    #+#             */
/*   Updated: 2018/02/16 23:08:12 by geargenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_COLORPICKER_H

# define FT_COLORPICKER_H

void	*ft_colorpicker(void *mlx_ptr, int (*f)(int, void *), void *param,
		char *name);
void	ft_close_colorpicker(void *clrpick_ptr);
void	ft_reset_colorpicker(void *clrpick_ptr);

#endif
