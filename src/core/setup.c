#include "main.h"

int	rt_setup(t_rt_env *env, int argc, char **argv)
{
	int		ret;

	(void)argc;
	(void)argv;
	if ((ret = init_mlx(env)))
		return (ret);
	return (0);
}
