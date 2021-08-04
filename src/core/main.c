#include "main.h"

static void	rt_loop(t_rt_env *env)
{
	void			*win;

	win = env->mlx.mlx_win;
	mlx_hook(win, 2, E_KEY_PRESS, (int(*)(void))key_press, env);
	mlx_hook(win, 3, E_KEY_RELEASE, (int(*)(void))key_release, env);
	mlx_hook(win, 4, E_MOUSE_PRESS, (int(*)(void))mouse_press, env);
	mlx_hook(win, 5, E_MOUSE_RELEASE, (int(*)(void))mouse_release, env);
	mlx_hook(win, 6, E_MOUSE_POSITION, (int(*)(void))mouse_position, env);
	mlx_hook(win, 17, E_CLOSE, (int(*)(void))exit_rt, env);
	mlx_loop_hook(env->mlx.mlx_ptr, (int(*)(void))render, env);
	mlx_loop(env->mlx.mlx_ptr);
}

static int	rt(t_rt_env	*env, int argc, char **argv)
{
	int		ret;

	if ((ret = rt_setup(env, argc, argv)) != 0)
		return (ret);

	rt_loop(env);
	return (0);
}

int			main(int argc, char **argv)
{
	t_rt_env	env;
	int			exit_code;

	ft_memset(&env, 0, sizeof(t_rt_env));
	exit_code = rt(&env, argc, argv);

	if (exit_code != 0)
		rt_error(exit_code);

	rt_free(&env);
	return (exit_code);
}
