#include "main.h"

int	rt_setup(t_rt_env *env, int argc, char **argv)
{
	int			ret;

	if (argc != 2)
		return (ERROR_NO_SCENE);

	if ((ret = parse_scene(env, argv[1]))
		|| (ret = init_opencl(env))
		|| (ret = init_mlx(env)))
		return (ret);

	return (0);
}
