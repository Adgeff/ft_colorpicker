# ft_colorpicker
Color-picker interface for minilibX

# User informations :

#include "ft_colorpicker.h"

void  *ft_colorpicker(void *mlx_ptr, int (*f)(int, void *), void *param, char *name);

void  ft_close_colorpicker(void *clrpick_ptr);

void  ft_reset_colorpicker(void *clrpick_ptr);

# General informations :

The function ft_colorpicker will open a new window which contains the color_picker interface.
You can edit WIN_WIDTH and WIN_HEIGHT macros in the file ft_clrpick.h to change the size of the
window : it should be able to adapt to any dimensions.
The project contains 2 headers (ft_clrpick.h and ft_colorpicker.h) :
 - The ft_clrpick.h file is used by the ft_colorpicker function to work and you are not supposed
to include it in you own projects. In this way you can have macros and structures with the same
names.
 - The ft_colorpicker.h file should be included to your files to be used. It only contains
prototypes of ft_colorpicker and ft_close_colorpicker.

To use the ft_colorpicker project you have to compile the following files :\
ft_bar.c\
ft_clrpick_event.c\
ft_colorpicker.c\
ft_cursor.c\
ft_imgtools.c\
ft_sqr.c

# Behavior :
The ft_colorpicker function needs 4 parameters to work :
 - void *mlx_ptr : the return value of mlx_init function that you have to call before using ft_colorpicker.
 - int (*f)(int, void *) : the function to call when a color is chosen. The int is the color chosen, the
void * is a parameter you can give in the third parameter of the ft_colorpicker function.
 - void *param : a parameter to give to your function you send in second parameter.
 - char *name : the name of the window created.

The ft_close_colorpicker function destoys window, images and free memory used by an ft_colorpicker call. The
function needs 1 parameter :
 - void *clrpick_ptr : The return value of the ft_colorpicker function.
 
The ft_reset_colorpicker function resets the color picker window, as a new one. It doesn't call the f function
pointer ! The function needs 1 parameter :
 - void *clrpick_ptr : The return value of the ft_colorpicker function.

# Return Values :
The ft_colorpicker function returns a void * pointer. You can use it to call the ft_close_colorpicker
function.
The ft_close_colorpicker and ft_reset_colorpicker functions do not return anything.

# Here is the list of headers includes in ft_clrpick.h :

#include "mlx.h"\
#include <stdlib.h>

# Here the list of macros in ft_clrpick.h :

#define FT_CLRPICK_H

#define WIN_WIDTH 600\
#define WIN_HEIGHT 400\
#define GREY 0x00A2B5BF

#define NOEVENTMASK 0L\
#define KEYPRESSMASK (1L<<0)\
#define KEYRELEASEMASK (1L<<1)\
#define BUTTONPRESSMASK (1L<<2)\
#define BUTTONRELEASEMASK (1L<<3)\
#define ENTERWINDOWMASK (1L<<4)\
#define LEAVEWINDOWMASK (1L<<5)\
#define POINTERMOTIONMASK (1L<<6)\
#define POINTERMOTIONHINTMASK (1L<<7)\
#define BUTTON1MOTIONMASK (1L<<8)\
#define BUTTON2MOTIONMASK (1L<<9)\
#define BUTTON3MOTIONMASK (1L<<10)\
#define BUTTON4MOTIONMASK (1L<<11)\
#define BUTTON5MOTIONMASK (1L<<12)\
#define BUTTONMOTIONMASK (1L<<13)\
#define KEYMAPSTATEMASK (1L<<14)\
#define EXPOSUREMASK (1L<<15)\
#define VISIBILITYCHANGEMASK (1L<<16)\
#define STRUCTURENOTIFYMASK (1L<<17)\
#define RESIZEREDIRECTMASK (1L<<18)\
#define SUBSTRUCTURENOTIFYMASK (1L<<19)\
#define SUBSTRUCTUREREDIRECTMASK (1L<<20)\
#define FOCUSCHANGEMASK (1L<<21)\
#define PROPERTYCHANGEMASK (1L<<22)\
#define COLORMAPCHANGEMASK (1L<<23)\
#define OWNERGRABBUTTONMASK (1L<<24)

#define KEYPRESS 2\
#define KEYRELEASE 3\
#define BUTTONPRESS 4\
#define BUTTONRELEASE 5\
#define MOTIONNOTIFY 6\
define ENTERNOTIFY 7\
#define LEAVENOTIFY 8\
#define FOCUSIN 9\
#define FOCUSOUT 10\
#define KEYMAPNOTIFY 11\
#define EXPOSE 12\
#define GRAPHICSEXPOSE 13\
#define NOEXPOSE 14\
#define VISIBILITYNOTIFY 15\
#define CREATENOTIFY 16\
#define DESTROYNOTIFY 17\
#define UNMAPNOTIFY 18\
#define MAPNOTIFY 19\
#define MAPREQUEST 20\
#define REPARENTNOTIFY 21\
#define CONFIGURENOTIFY 22\
#define CONFIGUREREQUEST 23\
#define GRAVITYNOTIFY 24

# Here the list of structures defined in clrpick.h :

typedef struct		s_img\
{\
	void			      *ptr;\
	char		      	*data;\
	int			      	bpp;\
	int			      	sl;\
	int		      		endian;\
	int		      		x_size;\
	int		      		y_size;\
	int		       		x;\
	int			      	y;\
}					        t_img;

typedef struct		s_click\
{\
	int				      button;\
	int			      	x;\
	int			      	y;\
}				        	t_click

typedef union		  u_color\
{\
	int				      color;\
	unsigned char	  byte[4];\
}				          t_color;

typedef struct		s_clrpick\
{\
	void		      	*mlx_ptr;\
	void		      	*win_ptr;\
	t_img		      	bar;\
	t_img		      	sqr;\
	t_img	      		bar_cursor;\
	t_img	      		sqr_cursor;\
	int			      	(*f)(int, void *);\
	void	      		*param;\
	t_click	     		click;\
}				        	t_clrpick;

# Here the list of non static functions defined :

void              *ft_colorpicker(void *mlx_ptr, int (*f)(int, void *), void *param, char *name);\
void              ft_close_colorpicker(void *clrpick_ptr);\

int				      	ft_bar(t_clrpick *clrpick);\
int				      	ft_bar_cursor(t_clrpick *clrpick);\
int			      		ft_clrpick_click_event(int button, int x, int y, void *param);\
int			      		ft_clrpick_motion_event(int x, int y, void *param);\
int			      		ft_clrpick_release_event(int button, int x, int y, void *param);\
int				      	ft_draw_sqr(t_clrpick *clrpick, t_color color);\
void			      	ft_draw_window(t_clrpick *clrpick);\
int				      	ft_isinimage(t_img *img, int x, int y);\
int				      	ft_sqr(t_clrpick *clrpick);\
int			      		ft_sqr_cursor(t_clrpick *clrpick);\
t_color		    		img_get_color(t_img *img, int x, int y);\
void			      	img_pixel_put(t_img *img, int x, int y, int color);
