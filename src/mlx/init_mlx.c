#include "main.h"

int		init_mlx(t_rt_env *env)
{
	t_mlx	*mlx;
	int		t;

	ft_putendl("Launching MinilibX...");
	mlx = &env->mlx;
	if (!(mlx->mlx_ptr = mlx_init())
		|| !(mlx->mlx_win = mlx_new_window(mlx->mlx_ptr, mlx->width, mlx->height, "RT"))
		|| !(mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, mlx->width, mlx->height))
		|| !(mlx->img_data = mlx_get_data_addr(mlx->img_ptr, &t, &t, &t)))
		return (ERROR_MLX_LAUNCH);
	return (0);
}
