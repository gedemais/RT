#ifndef MAIN_H
# define MAIN_H

# include "types_definition.h"
# include "libft.h"
# include "mlx.h"
# include "vectors.h"



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
	int			bpp;
	int			s_l;
	int			endian;
	int			pad;
};

struct	s_rt_env
{
	t_mlx	mlx;
};

int		rt_setup(t_rt_env *env, int argc, char **argv);

int		init_mlx(t_rt_env *env);

#endif
