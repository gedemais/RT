#ifndef MAIN_H
# define MAIN_H

# include "types_definition.h"
# include "opencl.h"
# include "libft.h"
# include "mlx.h"
# include "vectors.h"
# include "errors.h"

# include <stdio.h>
# include <sys/stat.h>
# include <sys/mman.h>

#ifdef MACOS
# include "macos_keys.h"
#endif

#ifdef LINUX
# include "linux_keys.h" // To add
#endif

struct			s_mlx
{
	bool		keys[NB_KEYS];
	bool		buttons[BUTTON_MAX];
	t_point2d	mouse_pos;
	void		*mlx_ptr;
	void		*mlx_win;
	void		*img_ptr;
	char		*img_data;
	uint32_t	width;
	uint32_t	height;
	int			bpp;
	int			s_l;
	int			endian;
	int			pad;
};

struct	s_rt_env
{
	t_mlx		mlx;
	t_opencl	cl_env;
};

int		rt_setup(t_rt_env *env, int argc, char **argv);
void	rt_error(int exit_code);
void	rt_free(t_rt_env *env);

int		init_mlx(t_rt_env *env);
int		key_press(int key, void *param);
int		key_release(int key, void *param);
int		mouse_press(int button, int x, int y, void *param);
int		mouse_release(int button, int x, int y, void *param);
int		mouse_position(int x, int y, void *param);
int		render(void *param);
int		exit_rt(t_rt_env *env);

void	draw_pixel(t_mlx *mlx, uint32_t x, uint32_t y, int color);

int			init_opencl(t_rt_env *env);
const char	*clGetErrorString(cl_int error);

#endif
