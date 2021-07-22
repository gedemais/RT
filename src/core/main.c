#include "main.h"

static int	rt(t_rt_env	*env, int argc, char **argv)
{
	int		ret;

	if ((ret = rt_setup(env, argc, argv)) != 0)
		return (ret);
	return (0);
}

int			main(int argc, char **argv)
{
	t_rt_env	env;
	int			ret;

	ret = rt(&env, argc, argv);
	return (ret);
}
