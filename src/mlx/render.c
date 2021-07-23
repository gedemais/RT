#include "main.h"

int		render(void *param)
{
	t_rt_env	*env;
	t_mlx		*mlx;

	env = (t_rt_env*)param;
	mlx = &env->mlx;

	mlx_put_image_to_window(mlx->mlx_ptr, mlx->mlx_win, mlx->img_ptr, 0, 0);
	return (0);
}
