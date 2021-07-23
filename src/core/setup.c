#include "main.h"

int	rt_setup(t_rt_env *env, int argc, char **argv)
{
	int		ret;

	(void)argc;
	(void)argv;

	// tmp data fill in
	env->mlx.width = 400;
	env->mlx.height = 400;
	//

	if ((ret = init_mlx(env))
		|| (ret = init_opencl(env)))
		return (ret);
	return (0);
}
