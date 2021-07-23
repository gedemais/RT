#include "main.h"

int		key_press(int key, void *param)
{
	t_rt_env	*env;

	env = (t_rt_env*)param;
	env->mlx.keys[key] = true;
	return (0);
}

int		key_release(int key, void *param)
{
	t_rt_env	*env;

	env = (t_rt_env*)param;
	env->mlx.keys[key] = false;
	return (0);
}

int		mouse_press(int button, int x, int y, void *param)
{
	t_rt_env	*env;

	(void)x;
	(void)y;
	env = (t_rt_env*)param;
	env->mlx.buttons[button] = true;
	return (0);
}

int		mouse_release(int button, int x, int y, void *param)
{
	t_rt_env	*env;

	(void)x;
	(void)y;
	env = (t_rt_env*)param;
	env->mlx.buttons[button] = false;
	return (0);
}

int		mouse_position(int x, int y, void *param)
{
	t_rt_env	*env;

	env = (t_rt_env*)param;
	env->mlx.mouse_pos = (t_point2d){x, y};
	return (0);
}

int		exit_rt(t_rt_env *env)
{
	rt_free(env);
	exit(EXIT_SUCCESS);
}
