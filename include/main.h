#ifndef MAIN_H
# define MAIN_H

# include "types_definition.h"
# include "vectors.h"
# include "opencl.h"
# include "errors.h"
# include "objects.h"

# include "libft.h"
# include "mlx.h"

# include <stdio.h>
# include <sys/stat.h>
# include <sys/mman.h>

#ifdef MACOS
# include "macos_keys.h"
#endif

#ifdef LINUX
# include "linux_keys.h" // To add
#endif

# define NB_SCENE_VARS 6
# define NB_COMMANDS 2
# define MAX_WINDOW_DIM 4320

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

struct	s_scene
{
	t_camera	cam;
	t_dynarray	objects;
	t_dynarray	lights;
};

struct	s_rt_env
{
	t_mlx		mlx;
	t_opencl	cl_env;
	t_scene		scene;
};

int		rt_setup(t_rt_env *env, int argc, char **argv);
void	rt_error(int exit_code);
void	rt_free(t_rt_env *env);
int		exit_rt(t_rt_env *env);

int		init_mlx(t_rt_env *env);
int		key_press(int key, void *param);
int		key_release(int key, void *param);
int		mouse_press(int button, int x, int y, void *param);
int		mouse_release(int button, int x, int y, void *param);
int		mouse_position(int x, int y, void *param);
int		render(void *param);
void	draw_pixel(t_mlx *mlx, uint32_t x, uint32_t y, int color);


int		parse_scene(t_rt_env *env, char *arg);

int		cmd_addobj(t_rt_env *env, char *line);
int		cmd_addlight(t_rt_env *env, char *line);
int		add_object_to_scene(t_rt_env *env, t_object *obj);

int		add_sphere(t_rt_env *env, char **split);
int		add_polygon(t_rt_env *env, char **split);

int		get_variables(t_rt_env *env, char **lines);
int		parse_window_dim(t_rt_env *env, char *line);
int		parse_cam_position(t_rt_env *env, char *line);
int		parse_fov(t_rt_env *env, char *line);
int		parse_brightness(t_rt_env *env, char *line);
int		parse_ambiant_color(t_rt_env *env, char *line);

int		parse_vector(char *vec, char *seps, cl_float3 *ret);

#endif
