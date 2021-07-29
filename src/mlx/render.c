#include "main.h"

static void	move(t_rt_env *env, t_mlx *mlx)
{
	t_camera	*cam;

	cam = (t_camera*)&env->scene.cam;

	if (mlx->keys[KEY_W])
		cam->o.z += CAM_SPEED;
	if (mlx->keys[KEY_S])
		cam->o.z -= CAM_SPEED;
	if (mlx->keys[KEY_A])
		cam->o.x += CAM_SPEED;
	if (mlx->keys[KEY_D])
		cam->o.x -= CAM_SPEED;
}

static void	handle_keys(t_rt_env *env, t_mlx *mlx)
{
	move(env, mlx);
}

int			render(void *param)
{
	t_rt_env	*env;
	t_mlx		*mlx;

	env = (t_rt_env*)param;
	mlx = &env->mlx;

	handle_keys(env, mlx);
	launch_ray_caster_kernel(env);
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->mlx_win, mlx->img_ptr, 0, 0);
	return (0);
}
